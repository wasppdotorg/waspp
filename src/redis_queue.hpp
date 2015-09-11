//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_REDIS_QUEUE_HPP
#define WASPP_REDIS_QUEUE_HPP

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>

#include "utility.hpp"

namespace waspp
{

	class redis_queue
		: public singleton<redis_queue>
	{
	public:
		redis_queue();
		~redis_queue();

		bool init();

	private:
		void init_impl();
		
		boost::asio::io_service io_service_;
		boost::scoped_ptr<boost::asio::io_service::work> work_;
		boost::scoped_ptr<boost::thread> thread_;

	};

} // namespace waspp

#endif // WASPP_REDIS_QUEUE_HPP
