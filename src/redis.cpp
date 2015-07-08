/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef _WIN32

#include <vector>
#include <string>

#include <boost/unordered_map.hpp>
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
			boost::unordered_map<std::string, std::string>& cfg_rd_shard = cfg->get("rd_shard");

			std::vector<std::string> keys;
			{
				keys.push_back("rd_shard_count");
				keys.push_back("rd_shard_format");
			}

			boost::unordered_map<std::string, std::string>::iterator found;
			for (std::size_t i = 0; i < keys.size(); ++i)
			{
				found = cfg_rd_shard.find(keys[i]);
				if (found == cfg_rd_shard.end())
				{
					return false;
				}

				if (keys[i] == "rd_shard_count")
				{
					rd_shard_count = boost::lexical_cast<unsigned int>(found->second);
				}
				else if (keys[i] == "rd_shard_format")
				{
					rd_shard_format = found->second;
				}
			}

			for (std::size_t i = 0; i < rdnames.size(); ++i)
			{
				rdpool_ptr rdpool(new rdconn_pool());

				if (!rdpool->init_pool(cfg->get(rdnames[i])) || !rdpool->fill_pool())
				{
					return false;
				}

				rd_.insert(std::make_pair(rdnames[i], rdpool));
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
		boost::unordered_map<std::string, rdpool_ptr>::iterator found;
		found = rd_.find(rdname);

		if (found == rd_.end())
		{
			throw std::runtime_error("invalid rdname");
		}

		return found->second;
	}

	scoped_rd::scoped_rd(redis* rd_, const std::string& rdname_) : rd(rd_)
	{
		rdpool = rd->get_rdpool(rdname_);
		rdconn = rdpool->get_rdconn();
	}

	scoped_rd::scoped_rd(redis* rd_, unsigned long long int shard_key_) : rd(rd_)
	{
		rdpool = rd->get_rdpool(shard_key_);
		rdconn = rdpool->get_rdconn();
	}

	scoped_rd::~scoped_rd()
	{
		rdpool->free_rdconn(rdconn);
	}

	rdconn_ptr scoped_rd::get()
	{
		return rdconn;
	}

} // namespace waspp

#endif // _WIN32