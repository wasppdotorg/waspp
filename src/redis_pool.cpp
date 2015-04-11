/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "redis_pool.hpp"

namespace waspp
{

	redis_pool::redis_pool()
		: port(0), pool_size(0), timeout_sec(0), pool(0)
	{
	}

	redis_pool::~redis_pool()
	{
	}

	bool redis_pool::init_pool(std::vector<name_value>& cfg)
	{
		std::vector<std::string> keys;
		{
			keys.push_back("host");
			keys.push_back("port");
			keys.push_back("pool_size");
			keys.push_back("timeout_sec");
		}

		std::vector<name_value>::iterator found;
		for (std::size_t i = 0; i < keys.size(); ++i)
		{
			found = std::find_if(cfg.begin(), cfg.end(), boost::bind(&name_value::compare_name, _1, keys[i]));
			if (found == cfg.end())
			{
				return false;
			}

			if (keys[i] == "host")
			{
				host = found->value;
			}
			else if (keys[i] == "port")
			{
				port = boost::lexical_cast<unsigned int>(found->value);
			}
			else if (keys[i] == "pool_size")
			{
				pool_size = boost::lexical_cast<std::size_t>(found->value);
			}
			else if (keys[i] == "timeout_sec")
			{
				timeout_sec = boost::lexical_cast<double>(found->value);
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
			redis_ptr r_conn = connect();
			if (!r_conn->is_valid())
			{
				return false;
			}

			pool.push_back(r_conn);
		}

		return true;
	}

	redis_ptr redis_pool::get_redis()
	{
		lock.acquire();
		//{
		if (pool.empty())
		{
			lock.release();
			return connect(false);
		}

		redis_ptr r_conn = *(pool.end() - 1);
		pool.pop_back();
		//}
		lock.release();

		double diff = std::difftime(std::time(0), mktime(r_conn->last_released()));

		if (diff > timeout_sec && !r_conn->is_valid())
		{
			return connect();
		}

		return r_conn;
	}

	void redis_pool::free_redis(redis_ptr r_conn)
	{
		if (!r_conn->is_pooled())
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

	rdconn_ptr rdconn_pool::connect(bool pooled_)
	{
		return rdconn_ptr(new mysqlpp::connection(host.c_str(), userid.c_str(), passwd.c_str(), database.c_str(), port, charset.c_str(), pooled_));
	}

	bool rdconn_pool::validate(rdconn_ptr rdconn)
	{
		try
		{
			boost::scoped_ptr<mysqlpp::statement> stmt(rdconn->prepare("DO 0"));
			stmt->execute();

			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

} // namespace waspp
