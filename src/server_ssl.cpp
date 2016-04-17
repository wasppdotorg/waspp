//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>
#include <unordered_map>

#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "server_ssl.hpp"

namespace waspp
{

	server_ssl::server_ssl(config* cfg_)
		: cfg(cfg_),
		context_(boost::asio::ssl::context::sslv23),
		signals_(io_service_),
		acceptor_(io_service_),
		new_connection_ssl_(),
		request_handler_()
	{
		try
		{
			context_.set_default_verify_paths();
			context_.set_options(boost::asio::ssl::context::default_workarounds);

			context_.use_certificate_chain_file(cfg->ssl_crt());
			context_.use_private_key_file(cfg->ssl_key(), boost::asio::ssl::context::pem);
		}
		catch (...)
		{

		}

		// Register to handle the signals that indicate when the server_ssl should exit.
		// It is safe to register for the same signal multiple times in a program,
		// provided all registration for the specified signal is made through Asio.
#if defined(SIGHUP)
		signals_.add(SIGHUP); // 1
#endif // defined(SIGHUP)
		signals_.add(SIGINT); // 2
#if defined(SIGQUIT)
		signals_.add(SIGQUIT); // 3
#endif // defined(SIGQUIT)
		signals_.add(SIGTERM); // 15

		signals_.async_wait(boost::bind(&server_ssl::handle_stop, this, boost::asio::placeholders::signal_number));

		// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
		boost::asio::ip::tcp::resolver resolver_(io_service_);
		boost::asio::ip::tcp::resolver::query query_(cfg->address(), cfg->port());
		boost::asio::ip::tcp::endpoint endpoint_ = *resolver_.resolve(query_);

		acceptor_.open(endpoint_.protocol());
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::keep_alive(true));
		acceptor_.bind(endpoint_);
		acceptor_.listen();

		start_accept();
	}

	void server_ssl::start_accept()
	{
		new_connection_ssl_.reset(new connection_ssl(io_service_, context_, request_handler_));

		acceptor_.async_accept(new_connection_ssl_->socket().lowest_layer(),
			boost::bind(&server_ssl::handle_accept, this,
			boost::asio::placeholders::error));
	}

	void server_ssl::run()
	{
		// Create a pool of threads to run all of the io_services.
		boost::thread_group threads;
		for (std::size_t i = 0; i < cfg->num_threads(); ++i)
		{
			threads.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
		}

		// Wait for all threads in the pool to exit.
		threads.join_all();
	}

	void server_ssl::handle_accept(const boost::system::error_code& e)
	{
		if (!e)
		{
			new_connection_ssl_->start();
		}

		start_accept();
	}

	void server_ssl::handle_stop(int signal_number)
	{
#if defined(SIGHUP)
		if (signal_number == SIGHUP)
		{
			return;
		}
#endif // defined(SIGHUP)

		log(info) << "server_ssl stopping..";
		io_service_.stop();
	}

} // namespace waspp
