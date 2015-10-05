/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef _WIN32

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

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

	bool redis_pool::init_pool(boost::unordered_map<std::string, std::string>& cfg)
	{
		std::vector<std::string> keys;
		{
			keys.push_back("host");
			keys.push_back("port");
			keys.push_back("pool_size");
			keys.push_back("timeout_sec");
		}

		boost::unordered_map<std::string, std::string>::iterator found;
		for (std::size_t i = 0; i < keys.size(); ++i)
		{
			found = cfg.find(keys[i]);
			if (found == cfg.end())
			{
				return false;
			}

			if (keys[i] == "host")
			{
				host = found->second;
			}
			else if (keys[i] == "port")
			{
				port = boost::lexical_cast<unsigned int>(found->second);
			}
			else if (keys[i] == "pool_size")
			{
				pool_size = boost::lexical_cast<std::size_t>(found->second);
			}
			else if (keys[i] == "timeout_sec")
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
			rdconn_ptr rdconn = connect();
			if (!rdconn->is_valid())
			{
				return false;
			}

			pool.push_back(rdconn);
		}

		return true;
	}

	rdconn_ptr redis_pool::get_rdconn()
	{
		lock.acquire();
		//{
		if (pool.empty())
		{
			lock.release();

			log(warn) << "redis_pool is empty";
			return connect(false);
		}

		rdconn_ptr rdconn = *(pool.end() - 1);
		pool.pop_back();
		//}
		lock.release();

		double diff = std::difftime(std::time(0), mktime(rdconn->last_released()));

		if (diff >= timeout_sec && !rdconn->is_valid())
		{
			return connect();
		}

		return rdconn;
	}

	void redis_pool::free_rdconn(rdconn_ptr rdconn)
	{
		if (!rdconn->is_pooled())
		{
			return;
		}

		std::time_t time_ = std::time(0);
		rdconn->set_released(*std::localtime(&time_));

		lock.acquire();
		{
			pool.push_back(rdconn);
		}
		lock.release();
	}

	rdconn_ptr redis_pool::connect(bool pooled_)
	{
		return redis3m::connection::connect(host.c_str(), port, pooled_);
	}

} // namespace waspp

#endif // _WIN32
