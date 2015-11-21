//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>

#include "connection_ssl.hpp"
#include "request_handler.hpp"
#include "logger.hpp"

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
		request_.remote_endpoint = boost::lexical_cast<std::string>(socket_.lowest_layer().remote_endpoint());

		socket_.async_handshake(boost::asio::ssl::stream_base::server,
			strand_.wrap(
			boost::bind(&connection_ssl::handle_handshake, shared_from_this(),
			boost::asio::placeholders::error)));

		//log(debug) << "new connection_ssl," << request_.remote_endpoint;
	}

	void connection_ssl::handle_handshake(const boost::system::error_code& e)
	{
		if (!e)
		{
			socket_.async_read_some(boost::asio::buffer(buffer_),
				strand_.wrap(
				boost::bind(&connection_ssl::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
		}
	}

	void connection_ssl::handle_read(const boost::system::error_code& e,
		std::size_t bytes_transferred)
	{
		if (!e)
		{
			boost::tribool result;
			boost::tie(result, boost::tuples::ignore) = request_parser_.parse(
				request_, buffer_.data(), buffer_.data() + bytes_transferred);

			if (result)
			{
				request_.remote_endpoint = boost::lexical_cast<std::string>(socket_.lowest_layer().remote_endpoint());

				request_parser_.parse_params(request_);
				request_parser_.parse_cookies(request_);
				request_parser_.parse_content(request_);

				request_handler_.handle_request(request_, response_);
				boost::asio::async_write(socket_, response_.to_buffers(),
					strand_.wrap(
					boost::bind(&connection_ssl::handle_write, shared_from_this(),
					boost::asio::placeholders::error)));
			}
			else if (!result)
			{
				response_ = response::static_response(response::bad_request);
				boost::asio::async_write(socket_, response_.to_buffers(),
					strand_.wrap(
					boost::bind(&connection_ssl::handle_write, shared_from_this(),
					boost::asio::placeholders::error)));
			}
			else
			{
				socket_.async_read_some(boost::asio::buffer(buffer_),
					strand_.wrap(
					boost::bind(&connection_ssl::handle_read, shared_from_this(),
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred)));
			}
		}

		// If an error occurs then no new asynchronous operations are started. This
		// means that all shared_ptr references to the connection_ssl object will
		// disappear and the object will be destroyed automatically after this
		// handler returns. The connection_ssl class's destructor closes the socket.
	}

	void connection_ssl::handle_write(const boost::system::error_code& e)
	{
		if (!e)
		{
			// Initiate graceful connection_ssl closure.
			//boost::system::error_code ignored_ec;
			//socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);

			request_parser_.reset();
			response_ = response();
			request_ = request();

			socket_.async_read_some(boost::asio::buffer(buffer_),
				strand_.wrap(
				boost::bind(&connection_ssl::handle_read, shared_from_this(),
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)));
		}

		// No new asynchronous operations are started. This means that all shared_ptr
		// references to the connection_ssl object will disappear and the object will be
		// destroyed automatically after this handler returns. The connection_ssl class's
		// destructor closes the socket.
	}

} // namespace waspp
