/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_REDIS_POOL_HPP
#define WASPP_REDIS_POOL_HPP

#include <ctime>

#include <memory>
#include <vector>
#include <unordered_map>

#include <boost/noncopyable.hpp>

#include "redis3m.hpp"
#include "utility.hpp"

namespace waspp
{

	typedef std::shared_ptr<redis3m::connection> rdconn_ptr;

	class redis_pool
		: private boost::noncopyable
	{
	public:
		redis_pool();
		~redis_pool();

		bool init_pool(std::unordered_map<std::string, std::string>& cfg);
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

#endif // WASPP_REDIS_POOL_HPP
