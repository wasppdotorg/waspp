/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_REDIS_POOL_HPP
#define WASPP_REDIS_POOL_HPP

#ifndef _WIN32

#include <ctime>

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "redis3m.hpp"
#include "name_value.hpp"
#include "utility.hpp"

namespace waspp
{

	typedef boost::shared_ptr<redis3m::connection> rdconn_ptr;

	class redis_pool
		: private boost::noncopyable
	{
	public:
		redis_pool();
		~redis_pool();

		bool init_pool(std::vector<name_value>& cfg);
		bool fill_pool();

		rdconn_ptr get_rdconn();
		void free_rdconn(rdconn_ptr rdconn);

	private:
		rdconn_ptr connect(bool pooled_ = true);

		std::string host;
		unsigned int port;

		std::size_t pool_size;
		double timeout_sec;

		std::vector<rdconn_ptr> pool;
		spinlock lock;

	};

} // namespace waspp

#endif // _WIN32
#endif // WASPP_REDIS_POOL_HPP
