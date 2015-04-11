/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_REDIS_HPP
#define WASPP_REDIS_HPP

#include <vector>
#include <string>

#include "redis3m.hpp"
#include "redis_pool.hpp"
#include "utility.hpp"
#include "config.hpp"

namespace waspp
{

	typedef boost::shared_ptr<redis3m::redis_pool> redis_pool_ptr;

	struct rdpair
	{
		rdpair(const std::string& first_, rdpool_ptr second_) : first(first_), second(second_)
		{
		}

		bool compare_first(const std::string& first_)
		{
			return first == first_;
		}

		std::string first;
		rdpool_ptr second;
	};

	class redis
		: public singleton<redis>
	{
	public:
		redis();
		~redis();

		bool init(config* cfg, const std::vector<std::string>& rdnames);

		rdpool_ptr get_rdpool(const std::string& rdname);

	private:
		std::vector<rdpair> rd_;

	};

	class scoped_rd
	{
	public:
		scoped_rd(redis* rd_, const std::string& rdname_);
		
		~scoped_rd();

		rdconn_ptr get();
		
	private:
		redis* rd;

		std::string rdname;

		rdconn_ptr rdconn;

	};

} // namespace waspp

#endif // WASPP_REDIS_HPP
