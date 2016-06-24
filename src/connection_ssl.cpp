//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "connection_ssl.hpp"

namespace waspp
{

	connection_ssl::connection_ssl(boost::asio::io_service& io_service,
		boost::asio::ssl::context& context,
		request_handler& handler)
		: strand_(io_service),
		socket_(io_service, context),
		request_handler_(handler)
	{
	}

	boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& connection_ssl::socket()
	{
		return socket_;
	}

	void connection_ssl::start()
	{
		do_handshake();
		//log(debug) << "new connection_ssl," << request_.remote_addr;
	}

	void connection_ssl::do_handshake()
	{
		auto self(shared_from_this());

		socket_.async_handshake(boost::asio::ssl::stream_base::server,
			strand_.wrap(
			[this, self](boost::system::error_code e)
		{
			if (!e)
			{
				do_read();
			}
		}));
	}

	void connection_ssl::do_read()
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
					request_.remote_addr = socket_.lowest_layer().remote_endpoint().address().to_string();
					request_.remote_port = socket_.lowest_layer().remote_endpoint().port();

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
		// means that all shared_ptr references to the connection_ssl object will
		// disappear and the object will be destroyed automatically after this
		// handler returns. The connection_ssl class's destructor closes the socket.
	}

	void connection_ssl::do_write()
	{
		auto self(shared_from_this());

		boost::asio::async_write(socket_, response_.to_buffers(),
			strand_.wrap(
			[this, self](boost::system::error_code e, std::size_t bytes_transferred)
		{
			if (!e)
			{
				request_parser_.reset();
				request_ = request();
				response_ = response();

				do_read();
			}
		}));

		// No new asynchronous operations are started. This means that all shared_ptr
		// references to the connection_ssl object will disappear and the object will be
		// destroyed automatically after this handler returns. The connection_ssl class's
		// destructor closes the socket.
	}

} // namespace waspp
