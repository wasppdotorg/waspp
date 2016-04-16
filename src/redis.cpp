/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <vector>
#include <string>

#include <boost/unordered_map.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "redis.hpp"
#include "config.hpp"
#include "redis_pool.hpp"
#include "redis3m.hpp"

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

			//boost::unordered_map<std::string, std::string>::iterator found;
			for (auto key : keys)
			{
				auto found = cfg_rd_shard.find(key);
				if (found == cfg_rd_shard.end())
				{
					return false;
				}

				if (key == "rd_shard_count")
				{
					rd_shard_count = boost::lexical_cast<unsigned int>(found->second);
				}
				else if (key == "rd_shard_format")
				{
					rd_shard_format = found->second;
				}
			}

			for (auto rdname : rdnames)
			{
				rdpool_ptr rdpool(new redis_pool());

				if (!rdpool->init_pool(cfg->get(rdname)) || !rdpool->fill_pool())
				{
					return false;
				}

				rd_.insert(std::make_pair(rdname, rdpool));
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
		//boost::unordered_map<std::string, rdpool_ptr>::iterator found;
		auto found = rd_.find(rdname);

		if (found == rd_.end())
		{
			throw std::runtime_error("invalid rdname");
		}

		return found->second;
	}

	rdpool_ptr redis::get_rdpool(unsigned long long int shard_key)
	{
		char format[8] = { 0 };

		int count = sprintf(format, rd_shard_format.c_str(), shard_key % rd_shard_count);
		if (count == 0)
		{
			throw std::runtime_error("invalid rd_shard_format");
		}

		//boost::unordered_map<std::string, rdpool_ptr>::iterator found;
		auto found = rd_.find(std::string(format));

		if (found == rd_.end())
		{
			throw std::runtime_error("invalid rd_shard_key");
		}

		return found->second;
	}

	scoped_rd::scoped_rd(redis* rd, const std::string& rdname)
	{
		rdpool = rd->get_rdpool(rdname);
		rdconn = rdpool->get_rdconn();
	}

	scoped_rd::scoped_rd(redis* rd, unsigned long long int shard_key)
	{
		rdpool = rd->get_rdpool(shard_key);
		rdconn = rdpool->get_rdconn();
	}

	scoped_rd::~scoped_rd()
	{
		rdpool->free_rdconn(rdconn);
	}

	redis3m::reply scoped_rd::run(const std::vector<std::string>& args)
	{
		return rdconn->run(args);
	}

} // namespace waspp
