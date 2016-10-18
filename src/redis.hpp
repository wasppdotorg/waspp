/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef redis_hpp
#define redis_hpp

#include "redis_pool.hpp"
#include "locator.hpp"
#include "config.hpp"

namespace waspp
{

	class redis
		: public locator<redis>
	{
	public:
		redis();
		~redis();

		bool init(config& cfg, const std::vector<std::string>& rdnames);

		redis_pool& get_rdpool(const std::string& rdname);
		redis_pool& get_rdpool(unsigned long long int shard_key);

	private:
		unsigned int rd_shard_count;
		std::string rd_shard_format;

		std::unordered_map<std::string, redis_pool*> rd_;

	};

	class scoped_rd
	{
	public:
		scoped_rd(const std::string& rdname);
		scoped_rd(unsigned long long int shard_key);

		~scoped_rd();

		redis3m::connection* conn;

	private:
		redis_pool& rdpool;

	};

} // namespace waspp

#endif // redis_hpp
