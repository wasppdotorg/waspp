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

	enum class rdname_type : int
	{
		rd_rnk = 0,
	};

	class redis
		: public locator<redis>
	{
	public:
		redis();
		~redis();

		bool init(config& cfg, const std::vector<std::string>& rdnames);

		redis_pool& get_rdpool(rdname_type rdname);
		redis_pool& get_rdpool(uint64_t shard_key);
		redis_pool& get_rdpool(const std::string& shard_key);

	private:
		unsigned int rd_shard_count;
		std::string rd_shard_format;

		std::vector<redis_pool*> rd_;

	};

	class scoped_rd
	{
	public:
		scoped_rd(rdname_type rdname);
		scoped_rd(uint64_t shard_key);
		scoped_rd(const std::string& shard_key);

		~scoped_rd();

	private:
		redis_pool& rdpool;
		
	public:
		redis3m::connection* conn;

	};

} // namespace waspp

#endif // redis_hpp
