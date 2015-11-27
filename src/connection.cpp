//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

#include "connection.hpp"
#include "request_handler.hpp"
#include "logger.hpp"

namespace waspp
{

	connection::connection(boost::asio::io_service& io_service,
		request_handler& handler)
		: strand_(io_service),
		socket_(io_service),
		request_(new request()),
		request_handler_(handler),
		response_(new response())
	{
	}

	boost::asio::ip::tcp::socket& connection::socket()
	{
		return socket_;
	}

	void connection::start()
	{
		socket_.async_read_some(boost::asio::buffer(buffer_),
			strand_.wrap(
			boost::bind(&connection::handle_read, shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));

		//log(debug) << "new connection," << request_.remote_endpoint;
	}

	void connection::handle_read(const boost::system::error_code& e,
		std::size_t bytes_transferred)
	{
		if (!e)
		{
			boost::tribool result;
			boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
				request_, buffer_.data(), buffer_.data() + bytes_transferred);

			if (result)
			{
				request_->parse_remote_endpoint(boost::lexical_cast<std::string>(socket_.remote_endpoint()));
				request_->parse_connection_header();

				request_parser_.parse_params(request_);
				request_parser_.parse_cookies(request_);
				request_parser_.parse_content(request_);

				request_handler_.handle_request(request_, response_);
				boost::asio::async_write(socket_, response_->to_buffers(),
					strand_.wrap(
					boost::bind(&connection::handle_write, shared_from_this(),
					boost::asio::placeholders::error)));
			}
			else if (!result)
			{
				*response_.get() = response::static_response(response::bad_request);
				boost::asio::async_write(socket_, response_->to_buffers(),
					strand_.wrap(
					boost::bind(&connection::handle_write, shared_from_this(),
					boost::asio::placeholders::error)));
			}
			else
			{
				socket_.async_read_some(boost::asio::buffer(buffer_),
					strand_.wrap(
					boost::bind(&connection::handle_read, shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
			}
		}

		// If an error occurs then no new asynchronous operations are started. This
		// means that all shared_ptr references to the connection object will
		// disappear and the object will be destroyed automatically after this
		// handler returns. The connection class's destructor closes the socket.
	}

	void connection::handle_write(const boost::system::error_code& e)
	{
		if (!e)
		{
			if (request_->connection_option == 'C')
			{
				// Initiate graceful connection closure.
				boost::system::error_code ignored_ec;
				socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
				return;
			}

			request_parser_.reset();
			response_.reset();
			request_.reset();

			socket_.async_read_some(boost::asio::buffer(buffer_),
				strand_.wrap(
				boost::bind(&connection::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
		}

		// No new asynchronous operations are started. This means that all shared_ptr
		// references to the connection object will disappear and the object will be
		// destroyed automatically after this handler returns. The connection class's
		// destructor closes the socket.
	}

} // namespace waspp
