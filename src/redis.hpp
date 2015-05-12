/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_REDIS_HPP
#define WASPP_REDIS_HPP

#ifndef _WIN32

#include <vector>
#include <string>

#include <boost/unordered_map.hpp>

#include "redis_pool.hpp"
#include "utility.hpp"
#include "config.hpp"

namespace waspp
{

	typedef boost::shared_ptr<redis_pool> rdpool_ptr;

	class redis
		: public singleton<redis>
	{
	public:
		redis();
		~redis();

		bool init(config* cfg, const std::vector<std::string>& rdnames);

		rdpool_ptr get_rdpool(const std::string& rdname);

	private:
		boost::unordered_map<std::string, rdpool_ptr> rd_;

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

#endif // _WIN32
#endif // WASPP_REDIS_HPP
