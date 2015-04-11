/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef _WIN32

#include <vector>
#include <string>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "redis.hpp"
#include "config.hpp"
#include "redis_pool.hpp"

namespace waspp
{

	redis::redis()
	{
	}

	redis::~redis()
	{
	}

	bool redis::init(config* cfg, const std::vector<std::string>& rdnames)
	{
		try
		{
			for (std::size_t i = 0; i < rdnames.size(); ++i)
			{
				rdpool_ptr rdpool(new redis_pool());

				if (!rdpool->init_pool(cfg->get(rdnames[i])) || !rdpool->fill_pool())
				{
					return false;
				}

				rd_.push_back(rdpair(rdnames[i], rdpool));
			}

			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

	rdpool_ptr redis::get_rdpool(const std::string& rdname)
	{
		std::vector<rdpair>::iterator found;
		found = std::find_if(rd_.begin(), rd_.end(), boost::bind(&rdpair::compare_first, _1, rdname));

		if (found == rd_.end())
		{
			throw std::runtime_error("invalid rdname");
		}

		return found->second;
	}

	scoped_rd::scoped_rd(redis* rd_, const std::string& rdname_) : rd(rd_), rdname(rdname_)
	{
	}

	scoped_rd::~scoped_rd()
	{
		rdpool_ptr rdpool = rd->get_rdpool(rdname);
		rdpool->free_rdconn(rdconn);
	}

	rdconn_ptr scoped_rd::get()
	{
		if (!rdname.empty())
		{
			rdpool_ptr rdpool = rd->get_rdpool(rdname);
			rdconn = rdpool->get_rdconn();
		}
		else
		{
			throw std::runtime_error("invalid rdname");
		}

		return rdconn;
	}

} // namespace waspp

#endif // _WIN32