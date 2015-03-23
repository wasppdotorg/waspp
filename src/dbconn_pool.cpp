/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "dbconn_pool.hpp"

namespace waspp
{

	dbconn_pool::dbconn_pool()
		: port(0), pool_size(0), timeout_sec(0), pool(0)
	{
	}

	dbconn_pool::~dbconn_pool()
	{
	}

	bool dbconn_pool::init_pool(std::vector<name_value>& cfg)
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
			else if (keys[i] == "userid")
			{
				userid = found->value;
			}
			else if (keys[i] == "passwd")
			{
				passwd = found->value;
			}
			else if (keys[i] == "database")
			{
				database = found->value;
			}
			else if (keys[i] == "port")
			{
				port = boost::lexical_cast<unsigned int>(found->value);
			}
			else if (keys[i] == "charset")
			{
				charset = found->value;
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

	bool dbconn_pool::fill_pool()
	{
		if (pool_size == 0)
		{
			return false;
		}

		for (std::size_t i = 0; i < pool_size; ++i)
		{
			dbconn_ptr dbconn = connect();
			if (!validate(dbconn))
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
			return connect(false);
		}

		dbconn_ptr dbconn = *(pool.end() - 1);
		pool.pop_back();
		//}
		lock.release();

		double diff = std::difftime(std::time(0), mktime(dbconn->last_released()));

		if (diff > timeout_sec && !validate(dbconn))
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

	bool dbconn_pool::validate(dbconn_ptr dbconn)
	{
		try
		{
			boost::scoped_ptr<mysqlpp::statement> stmt(dbconn->prepare("DO 0"));
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
