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

	server::server(config* cfg_)
		: cfg(cfg_),
		signals_(io_service_),
		acceptor_(io_service_),
		new_connection_(),
		request_handler_()
	{
		// Register to handle the signals that indicate when the server should exit.
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

		signals_.async_wait(boost::bind(&server::handle_stop, this, boost::asio::placeholders::signal_number));

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

	void server::start_accept()
	{
		new_connection_.reset(new connection(io_service_, request_handler_));

		acceptor_.async_accept(new_connection_->socket(),
			boost::bind(&server::handle_accept, this,
			boost::asio::placeholders::error));
	}

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

	void server::handle_stop(int signal_number)
	{
#if defined(SIGHUP)
		if (signal_number == SIGHUP)
		{
			return;
		}
#endif // defined(SIGHUP)

		log(info) << "server stopping..";
		io_service_.stop();
	}

} // namespace waspp
