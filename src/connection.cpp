//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/lexical_cast.hpp>

#include "connection.hpp"
#include "request_handler.hpp"
#include "logger.hpp"

namespace waspp
{

	connection::connection(boost::asio::io_service& io_service,
		boost::asio::ip::tcp::socket socket,
		request_handler& handler)
		: strand_(io_service),
		socket_(std::move(socket)),
		request_handler_(handler)
	{
	}

	boost::asio::ip::tcp::socket& connection::socket()
	{
		return socket_;
	}

	void connection::start()
	{
		do_read();
		//log(debug) << "new connection," << request_.remote_addr;
	}

	void connection::do_read()
	{
		auto self(shared_from_this());

		socket_.async_read_some(boost::asio::buffer(buffer_),
			strand_.wrap(
				[this, self](boost::system::error_code e, std::size_t bytes_transferred)
		{
			if (!e)
			{
				boost::tribool result;
				boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
					request_, buffer_.data(), buffer_.data() + bytes_transferred);

				if (result)
				{
					request_.remote_addr = socket_.remote_endpoint().address().to_string();
					request_.remote_port = socket_.remote_endpoint().port();
					request_.parse_connection_header();

					request_parser_.parse_params(request_);
					request_parser_.parse_cookies(request_);
					request_parser_.parse_content(request_);

					request_handler_.handle_request(request_, response_);
					do_write();
				}
				else if (!result)
				{
					response_ = response::static_response(response::bad_request);
					do_write();
				}
				else
				{
					do_read();
				}
			}
		}));

		// If an error occurs then no new asynchronous operations are started. This
		// means that all shared_ptr references to the connection object will
		// disappear and the object will be destroyed automatically after this
		// handler returns. The connection class's destructor closes the socket.
	}

	void connection::do_write()
	{
		auto self(shared_from_this());

		boost::asio::async_write(socket_, response_.to_buffers(),
			strand_.wrap(
				[this, self](boost::system::error_code e, std::size_t bytes_transferred)
		{
			if (!e)
			{
				if (request_.connection_option == 'c')
				{
					// Initiate graceful connection closure.
					boost::system::error_code ignored_ec;
					socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
					return;
				}

				request_parser_.reset();
				request_ = request();
				response_ = response();

				do_read();
			}
		}));

		// No new asynchronous operations are started. This means that all shared_ptr
		// references to the connection object will disappear and the object will be
		// destroyed automatically after this handler returns. The connection class's
		// destructor closes the socket.
	}

} // namespace waspp
