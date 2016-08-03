/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef redis_pool_hpp
#define redis_pool_hpp

#include <memory>
#include <unordered_map>

#include "redis3m.hpp"
#include "spinlock.hpp"

namespace waspp
{

	using redis3m_ptr = std::shared_ptr<redis3m::connection>;

	class redis_pool
	{
	public:
		redis_pool(const redis_pool&) = delete;
		redis_pool& operator=(const redis_pool&) = delete;

		redis_pool();
		~redis_pool();

		bool init_pool(std::unordered_map<std::string, std::string>& cfg);
		bool fill_pool();

		redis3m_ptr get_rdconn();
		void free_rdconn(redis3m_ptr rdconn);

	private:
		redis3m_ptr connect(bool pooled_ = true);

		std::string host;
		unsigned int port;

		std::size_t pool_size;
		double timeout_sec;

		std::vector<redis3m_ptr> pool;
		spinlock spinlock_;

	};

} // namespace waspp

#endif // redis_pool_hpp
