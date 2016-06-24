//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef connection_ssl_hpp
#define connection_ssl_hpp

#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "request_handler.hpp"
#include "request_parser.hpp"

namespace waspp
{

	/// Represents a single connection_ssl from a client.
	class connection_ssl
		: public std::enable_shared_from_this<connection_ssl>
	{
	public:
		connection_ssl(const connection_ssl&) = delete;
		connection_ssl& operator=(const connection_ssl&) = delete;

		/// Construct a connection_ssl with the given io_service.
		explicit connection_ssl(boost::asio::io_service& io_service,
			boost::asio::ssl::context& context,
			request_handler& handler);

		/// Get the socket associated with the connection_ssl.
		boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket();

		/// Start the first asynchronous operation for the connection_ssl.
		void start();

	private:
		void do_handshake();

		/// Perform an asynchronous read operation.
		void do_read();

		/// Perform an asynchronous write operation.
		void do_write();

		/// Strand to ensure the connection_ssl's handlers are not called concurrently.
		boost::asio::io_service::strand strand_;

		/// Socket for the connection_ssl.
		boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;

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

	using connection_ssl_ptr = std::shared_ptr<connection_ssl>;

} // namespace waspp

#endif // connection_ssl_hpp
