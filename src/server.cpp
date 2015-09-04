//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>

#include "server.hpp"

namespace waspp
{

#ifdef CHECK_MEMORY_LEAK_WITH_SSL

	server::server(config* cfg_)
		: cfg(cfg_),
		context_(boost::asio::ssl::context::sslv23),
		signals_(io_service_),
		acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), cfg->port())),
		new_connection_(),
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

		// Register to handle the signals that indicate when the server should exit.
		// It is safe to register for the same signal multiple times in a program,
		// provided all registration for the specified signal is made through Asio.
		signals_.add(SIGINT);
		signals_.add(SIGTERM);
#if defined(SIGQUIT)
		signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
		signals_.async_wait(boost::bind(&server::handle_stop, this));

		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::keep_alive(true));

		start_accept();
	}

	void server::start_accept()
	{
		new_connection_.reset(new connection(io_service_, context_, request_handler_));

		acceptor_.async_accept(new_connection_->socket().lowest_layer(),
			boost::bind(&server::handle_accept, this,
			boost::asio::placeholders::error));
	}

#else

	server::server(config* cfg_)
		: cfg(cfg_),
		signals_(io_service_),
		acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), cfg->port())),
		new_connection_(),
		request_handler_()
	{
		// Register to handle the signals that indicate when the server should exit.
		// It is safe to register for the same signal multiple times in a program,
		// provided all registration for the specified signal is made through Asio.
		signals_.add(SIGINT);
		signals_.add(SIGTERM);
#if defined(SIGQUIT)
		signals_.add(SIGQUIT);
#endif // defined(SIGQUIT)
		signals_.async_wait(boost::bind(&server::handle_stop, this));

		acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		acceptor_.set_option(boost::asio::ip::tcp::acceptor::keep_alive(true));

		start_accept();
	}

	void server::start_accept()
	{
		new_connection_.reset(new connection(io_service_, request_handler_));

		acceptor_.async_accept(new_connection_->socket(),
			boost::bind(&server::handle_accept, this,
			boost::asio::placeholders::error));
	}

#endif

	void server::run()
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

	void server::handle_accept(const boost::system::error_code& e)
	{
		if (!e)
		{
			new_connection_->start();
		}

		start_accept();
	}

	void server::handle_stop()
	{
		log(info) << "server stopping..";
		io_service_.stop();
	}

} // namespace waspp
