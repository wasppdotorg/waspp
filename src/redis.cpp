/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include "redis.hpp"

namespace waspp
{

	redis::redis()
	{
		
	}

	redis::~redis()
	{
		for (auto& r : rd_)
		{
			delete r.second;
		}
	}

	bool redis::init(config& cfg, const std::vector<std::string>& rdnames)
	{
		try
		{
			auto cfg_rd_shard = cfg.get("rd_shard");

			std::vector<std::string> keys;
			//
				keys.push_back("rd_shard_count");
				keys.push_back("rd_shard_format");
			//

			for (auto& key : keys)
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

			for (auto& rdname : rdnames)
			{
				auto rdpool = new redis_pool();

				if (!rdpool->init_pool(cfg.get(rdname)) || !rdpool->fill_pool())
				{
					delete rdpool;
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

	redis_pool* redis::get_rdpool(const std::string& rdname)
	{
		auto found = rd_.find(rdname);
		if (found == rd_.end())
		{
			throw std::runtime_error("invalid rdname");
		}

		return found->second;
	}

	redis_pool* redis::get_rdpool(unsigned long long int shard_key)
	{
		char rdname[8] = { 0 };

		auto count_ = sprintf(rdname, rd_shard_format.c_str(), shard_key % rd_shard_count);
		if (count_ == 0)
		{
			throw std::runtime_error("invalid rd_shard_format");
		}

		auto found = rd_.find(rdname);
		if (found == rd_.end())
		{
			throw std::runtime_error("invalid rd_shard_key");
		}

		return found->second;
	}

	scoped_rd::scoped_rd(const std::string& rdname)
		: rdpool(*(redis::instance()->get_rdpool(rdname)))
	{
		ptr = rdpool.get_rdconn();
	}

	scoped_rd::scoped_rd(unsigned long long int shard_key)
		: rdpool(*(redis::instance()->get_rdpool(shard_key)))
	{
		ptr = rdpool.get_rdconn();
	}

	scoped_rd::~scoped_rd()
	{
		rdpool.free_rdconn(ptr);
	}

} // namespace waspp
