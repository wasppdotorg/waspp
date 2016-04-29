//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_CONNECTION_HPP
#define WASPP_CONNECTION_HPP

#include <memory>
#include <array>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "request_handler.hpp"
#include "request.hpp"
#include "request_parser.hpp"
#include "response.hpp"

namespace waspp
{

	/// Represents a single connection from a client.
	class connection
		: public std::enable_shared_from_this<connection>
	{
	public:
		connection(const connection&) = delete;
		connection& operator=(const connection&) = delete;

		/// Construct a connection with the given io_service.
		explicit connection(boost::asio::io_service& io_service,
			request_handler& handler);

		/// Get the socket associated with the connection.
		boost::asio::ip::tcp::socket& socket();

		/// Start the first asynchronous operation for the connection.
		void start();

	private:
		/// Perform an asynchronous read operation.
		void do_read();

		/// Perform an asynchronous write operation.
		void do_write();

		/// Strand to ensure the connection's handlers are not called concurrently.
		boost::asio::io_service::strand strand_;

		/// Socket for the connection.
		boost::asio::ip::tcp::socket socket_;

		/// The handler used to process the incoming request.
		request_handler& request_handler_;

		/// Buffer for incoming data.
		std::array<char, 8192> buffer_;

		/// The incoming request.
		request request_;

		/// The parser for the incoming request.
		request_parser request_parser_;

		/// The response to be sent back to the client.
		response response_;

	};

	using connection_ptr = std::shared_ptr<connection>;

} // namespace waspp

#endif // WASPP_CONNECTION_HPP
