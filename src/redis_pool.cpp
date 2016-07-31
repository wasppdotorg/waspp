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
				port = boost::lexical_cast<unsigned int>(found->second);
			}
			else if (key == "pool_size")
			{
				pool_size = boost::lexical_cast<std::size_t>(found->second);
			}
			else if (key == "timeout_sec")
			{
				timeout_sec = boost::lexical_cast<double>(found->second);
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
				return false;
			}

			pool.push_back(rdconn);
		}

		return true;
	}

	redis3m_ptr redis_pool::get_rdconn()
	{
		lock.acquire();
		//
			if (pool.empty())
			{
				lock.release();

				log(warn) << "redis_pool is empty";
				return connect(false);
			}

			auto rdconn = pool.back();
			pool.pop_back();
		//
		lock.release();

		auto diff = std::difftime(std::time(nullptr), mktime(rdconn->last_released()));
		if (diff >= timeout_sec && !rdconn->is_valid())
		{
			return connect();
		}

		return rdconn;
	}

	void redis_pool::free_rdconn(redis3m_ptr rdconn)
	{
		if (!rdconn->is_pooled())
		{
			return;
		}

		auto time_ = std::time(nullptr);
		rdconn->set_released(*std::localtime(&time_));

		lock.acquire();
		//
			pool.push_back(rdconn);
		//
		lock.release();
	}

	redis3m_ptr redis_pool::connect(bool pooled_)
	{
		return redis3m::connection::connect(host, port, pooled_);
	}

} // namespace waspp

