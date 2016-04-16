/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "dbconn_pool.hpp"
#include "logger.hpp"

namespace waspp
{

	dbconn_pool::dbconn_pool()
		: port(0), pool_size(0), wait_timeout_sec(0), pool(0)
	{
	}

	dbconn_pool::~dbconn_pool()
	{
	}

	bool dbconn_pool::init_pool(boost::unordered_map<std::string, std::string>& cfg)
	{
		std::vector<std::string> keys;
		{
			keys.push_back("host");
			keys.push_back("userid");
			keys.push_back("passwd");
			keys.push_back("database");
			keys.push_back("port");
			keys.push_back("charset");
			keys.push_back("pool_size");
			keys.push_back("wait_timeout_sec");
		}

		//boost::unordered_map<std::string, std::string>::iterator found;
		for (auto key : keys)
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
			else if (key == "userid")
			{
				userid = found->second;
			}
			else if (key == "passwd")
			{
				passwd = found->second;
			}
			else if (key == "database")
			{
				database = found->second;
			}
			else if (key == "port")
			{
				port = boost::lexical_cast<unsigned int>(found->second);
			}
			else if (key == "charset")
			{
				charset = found->second;
			}
			else if (key == "pool_size")
			{
				pool_size = boost::lexical_cast<std::size_t>(found->second);
			}
			else if (key == "wait_timeout_sec")
			{
				wait_timeout_sec = boost::lexical_cast<double>(found->second);
			}
		}

		return true;
	}

	bool dbconn_pool::fill_pool()
	{
		if (pool_size == 0)
		{
			return false;
		}

		for (std::size_t i = 0; i < pool_size; ++i)
		{
			dbconn_ptr dbconn = connect();
			if (!dbconn->ping())
			{
				return false;
			}

			pool.push_back(dbconn);
		}

		return true;
	}

	dbconn_ptr dbconn_pool::get_dbconn()
	{
		lock.acquire();
		//{
		if (pool.empty())
		{
			lock.release();

			log(warn) << "dbconn_pool is empty";
			return connect(false);
		}

		dbconn_ptr dbconn = pool.back();
		pool.pop_back();
		//}
		lock.release();

		double diff = std::difftime(std::time(0), mktime(dbconn->last_released()));

		if (diff >= wait_timeout_sec && !dbconn->ping())
		{
			return connect();
		}

		return dbconn;
	}

	void dbconn_pool::free_dbconn(dbconn_ptr dbconn)
	{
		if (!dbconn->is_pooled())
		{
			return;
		}

		std::time_t time_ = std::time(0);
		dbconn->set_released(*std::localtime(&time_));

		lock.acquire();
		{
			pool.push_back(dbconn);
		}
		lock.release();
	}

	dbconn_ptr dbconn_pool::connect(bool pooled_)
	{
		return dbconn_ptr(new mysqlpp::connection(host.c_str(), userid.c_str(), passwd.c_str(), database.c_str(), port, charset.c_str(), pooled_));
	}

} // namespace waspp
