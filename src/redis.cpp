/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/lexical_cast.hpp>
#include <boost/crc.hpp>

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

	bool redis::init(config& cfg, const std::unordered_map<int, std::string>& rdnames)
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

			for (auto& rdpair : rdnames)
			{
				auto rdpool = new redis_pool();

				if (!rdpool->init_pool(cfg.get(rdpair.second)) || !rdpool->fill_pool())
				{
					delete rdpool;
					return false;
				}

				rd_.insert(std::make_pair(rdpair.first, rdpool));
			}

			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

	redis_pool& redis::get_rdpool(rdname_type rdname)
	{
		auto found = rd_.find(rdname);
		if (found == rd_.end())
		{
			throw std::runtime_error("invalid rd_name_type");
		}

		return *found->second;
	}

	redis_pool& redis::get_rdpool(uint64_t shard_key)
	{
		auto found = rd_.find(shard_key % rd_shard_count);
		if (found == rd_.end())
		{
			throw std::runtime_error("invalid rd_shard_key");
		}

		return *found->second;
	}

	redis_pool& redis::get_rdpool(const std::string& shard_key)
	{
		boost::crc_32_type crc32;
		crc32.process_bytes(shard_key.c_str(), shard_key.size());
		
		return get_rdpool((uint64_t)crc32.checksum());
	}

	scoped_rd::scoped_rd(const std::string& rdname)
		: rdpool(redis::instance().get_rdpool(rdname))
	{
		conn = rdpool.get_rdconn();
	}

	scoped_rd::scoped_rd(uint64_t shard_key)
		: rdpool(redis::instance().get_rdpool(shard_key))
	{
		conn = rdpool.get_rdconn();
	}

	scoped_rd::~scoped_rd()
	{
		rdpool.release_rdconn(conn);
	}

} // namespace waspp
