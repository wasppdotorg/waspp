//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>

#include "redis_queue.hpp"

namespace waspp
{

	/// Constructor creates a thread to run a private io_service.
	redis_queue::redis_queue()
		: io_service_(),
		work_(new boost::asio::io_service::work(io_service_)),
		thread_(new boost::thread(
		boost::bind(&boost::asio::io_service::run, &io_service_)))
	{
		
	}

	/// Destructor shuts down the private io_service.
	redis_queue::~redis_queue()
	{
		/// Indicate that we have finished with the private io_service. Its
		/// io_service::run() function will exit once all other work has completed.
		work_.reset();

		if (thread_)
		{
			thread_->join();
		}
	}

	bool redis_queue::init()
	{	
		io_service_.post(boost::bind(&redis_queue::init_impl, this));
		return true;
	}

	void redis_queue::init_impl()
	{
	}

} // namespace waspp
