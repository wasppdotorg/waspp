/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_REDIS_HPP
#define WASPP_REDIS_HPP

#include <vector>
#include <string>
#include <unordered_map>

#include "redis_pool.hpp"
#include "utility.hpp"
#include "config.hpp"
#include "redis3m.hpp"

namespace waspp
{

	using rdpool_ptr = redis_pool*;

	class redis
		: public singleton<redis>
	{
	public:
		redis();
		~redis();

		bool init(config* cfg, const std::vector<std::string>& rdnames);

		rdpool_ptr get_rdpool(const std::string& rdname);
		rdpool_ptr get_rdpool(unsigned long long int shard_key);

	private:
		unsigned int rd_shard_count;
		std::string rd_shard_format;

		std::unordered_map<std::string, rdpool_ptr> rd_;

	};

	class scoped_rd
	{
	public:
		scoped_rd(redis* rd, const std::string& rdname);
		scoped_rd(redis* rd, unsigned long long int shard_key);

		~scoped_rd();

		redis3m::reply run(const std::vector<std::string>& args);

	private:
		rdpool_ptr rdpool;
		redis3m_ptr rdconn;

	};

} // namespace waspp

#endif // WASPP_REDIS_HPP
