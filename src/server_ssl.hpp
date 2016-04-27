//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_SERVER_SSL_HPP
#define WASPP_SERVER_SSL_HPP

#include <string>

#include <boost/asio.hpp>

#include "logger.hpp"
#include "config.hpp"
#include "connection_ssl.hpp"
#include "request_handler.hpp"

namespace waspp
{

	/// The top-level class of the HTTP server_ssl.
	class server_ssl
	{
	public:
		server_ssl(const server_ssl&) = delete;
		server_ssl& operator=(const server_ssl&) = delete;

		/// Construct the server_ssl to listen on the specified TCP address and port, and
		/// serve up files from the given directory.
		explicit server_ssl(config* cfg_);

		/// Run the server_ssl's io_service loop.
		void run();

	private:
		/// Initiate an asynchronous accept operation.
		void start_accept();

		/// Handle completion of an asynchronous accept operation.
		void handle_accept(const boost::system::error_code& e);

		/// Handle a request to stop the server_ssl.
		void handle_stop(int signal_number);

		config* cfg;

		/// The io_service used to perform asynchronous operations.
		boost::asio::io_service io_service_;

		boost::asio::ssl::context context_;

		/// The signal_set is used to register for process termination notifications.
		boost::asio::signal_set signals_;

		/// Acceptor used to listen for incoming connections.
		boost::asio::ip::tcp::acceptor acceptor_;

		/// The next connection to be accepted.
		connection_ssl_ptr new_connection_ssl_;

		/// The handler for all incoming requests.
		request_handler request_handler_;

	};

} // namespace waspp

#endif // WASPP_SERVER_SSL_HPP
