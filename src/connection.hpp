//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_CONNECTION_HPP
#define WASPP_CONNECTION_HPP

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/array.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "request_handler.hpp"
#include "request.hpp"
#include "request_parser.hpp"
#include "response.hpp"

namespace waspp
{

	/// Represents a single connection from a client.
	class connection
		: public boost::enable_shared_from_this<connection>,
		private boost::noncopyable
	{
	public:

#define CHECK_MEMORY_LEAK_WITHOUT_SSL
#ifdef  CHECK_MEMORY_LEAK_WITH_SSL

		/// Construct a connection with the given io_service.
		explicit connection(boost::asio::io_service& io_service,
			boost::asio::ssl::context& context,
			request_handler& handler);

		/// Get the socket associated with the connection.
		boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket();

#else

		/// Construct a connection with the given io_service.
		explicit connection(boost::asio::io_service& io_service,
			request_handler& handler);

		/// Get the socket associated with the connection.
		boost::asio::ip::tcp::socket& socket();

#endif

		/// Start the first asynchronous operation for the connection.
		void start();

	private:

#ifdef CHECK_MEMORY_LEAK_WITH_SSL

		void handle_handshake(const boost::system::error_code& e);

		/// Socket for the connection.
		boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;

#else

		/// Socket for the connection.
		boost::asio::ip::tcp::socket socket_;

#endif

		/// Handle completion of a read operation.
		void handle_read(const boost::system::error_code& e,
			std::size_t bytes_transferred);

		/// Handle completion of a write operation.
		void handle_write(const boost::system::error_code& e);

		/// Strand to ensure the connection's handlers are not called concurrently.
		boost::asio::io_service::strand strand_;

		/// The handler used to process the incoming request.
		request_handler& request_handler_;

		/// Buffer for incoming data.
		boost::array<char, 8192> buffer_;

		/// The incoming request.
		request request_;

		/// The parser for the incoming request.
		request_parser request_parser_;

		/// The response to be sent back to the client.
		response response_;
	};

	typedef boost::shared_ptr<connection> connection_ptr;

} // namespace waspp

#endif // WASPP_CONNECTION_HPP
