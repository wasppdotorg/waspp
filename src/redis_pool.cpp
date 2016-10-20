/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include "redis_pool.hpp"
#include "logger.hpp"

namespace waspp
{

	redis_pool::redis_pool()
		: port(0), pool_size(0), timeout_sec(0), pool(0)
	{
		
	}

	redis_pool::~redis_pool()
	{
		for (auto& p : pool)
		{
			delete p;
		}
	}

	bool redis_pool::init_pool(std::unordered_map<std::string, std::string>& cfg)
	{
		std::vector<std::string> keys;
		//
			keys.push_back("host");
			keys.push_back("port");
			keys.push_back("pool_size");
			keys.push_back("timeout_sec");
		//

		for (auto& key : keys)
		{
			auto found = cfg.find(key);
			if (found == cfg.end())
			{
				return false;
			}

			if (key == "host")
			{
				host = found->second;
			}
			else if (key == "port")
			{
				port = strtoul(found->second.c_str(), nullptr, 0);
			}
			else if (key == "pool_size")
			{
				pool_size = strtoul(found->second.c_str(), nullptr, 0);
			}
			else if (key == "timeout_sec")
			{
				timeout_sec = strtod(found->second.c_str(), nullptr);
			}
		}

		return true;
	}

	bool redis_pool::fill_pool()
	{
		if (pool_size == 0)
		{
			return false;
		}

		for (std::size_t i = 0; i < pool_size; ++i)
		{
			auto rdconn = connect();
			if (!rdconn->is_valid())
			{
				delete rdconn;
				return false;
			}

			pool.push_back(rdconn);
		}

		return true;
	}

	redis3m::connection* redis_pool::get_rdconn()
	{
		spinlock_.lock();
		//
			if (pool.empty())
			{
				spinlock_.unlock();

				log(warn) << "redis_pool is empty";
				return connect(false);
			}

			auto rdconn = pool.back();
			pool.pop_back();
		//
		spinlock_.unlock();

		auto diff = std::difftime(std::time(nullptr), mktime(rdconn->last_released()));
		if (diff >= timeout_sec && !rdconn->is_valid())
		{
			delete rdconn;
			return connect();
		}

		return rdconn;
	}

	void redis_pool::release_rdconn(redis3m::connection* rdconn)
	{
		if (!rdconn->is_pooled())
		{
			delete rdconn;
			return;
		}

		auto time_ = std::time(nullptr);
		rdconn->set_released(*std::localtime(&time_));

		spinlock_.lock();
		//
			pool.push_back(rdconn);
		//
		spinlock_.unlock();
	}

	redis3m::connection* redis_pool::connect(bool pooled_)
	{
		return redis3m::connection::connect(host, port, pooled_);
	}

} // namespace waspp

