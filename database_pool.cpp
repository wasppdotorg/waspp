//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/lexical_cast.hpp>

#include "database_pool.hpp"

namespace waspp
{

	database_pool::database_pool()
		: port(0), pool_size(0), timeout_sec(0), pool(0)
	{
	}

	database_pool::~database_pool()
	{
	}

	bool database_pool::init_pool(std::map<std::string, std::string> c)
	{
		if (c.find("host") == c.end() ||
			c.find("userid") == c.end() ||
			c.find("passwd") == c.end() ||
			c.find("database") == c.end() ||
			c.find("port") == c.end() ||
			c.find("pool_size") == c.end() ||
			c.find("timeout_sec") == c.end())
		{
			return false;
		}

		host = c["host"];
		userid = c["userid"];
		passwd = c["passwd"];
		database = c["database"];

		port = boost::lexical_cast<unsigned int>(c["port"]);
		pool_size = boost::lexical_cast<std::size_t>(c["pool_size"]);
		timeout_sec = boost::lexical_cast<double>(c["timeout_sec"]);

		return true;
	}

	bool database_pool::fill_pool()
	{
		if (pool_size == 0)
		{
			return false;
		}

		lock.set();
		{
			for (std::size_t i = 0; i < pool_size; ++i)
			{
				database_ptr db = connect();
				if (!validate(db))
				{
					lock.clear();
					return false;
				}

				pool.push_back(db);
			}
		}
		lock.clear();

		return true;
	}

	database_ptr database_pool::acquire_connection()
	{
		database_ptr db;

		lock.set();
		{
			if (pool.empty())
			{
				lock.clear();
				return connect(false);
			}

			db = *(pool.end() - 1);
		}
		lock.clear();

		std::time_t time_ = std::time(0);
		double diff = std::difftime(time_, mktime(db->last_released()));

		if (diff > timeout_sec && !validate(db))
		{
			return connect();
		}

		return db;
	}

	void database_pool::release_connection(database_ptr db)
	{
		if (!db->is_pooled())
		{
			return;
		}

		std::time_t time_ = std::time(0);
		db->set_released(*std::localtime(&time_));

		lock.set();
		{
			pool.push_back(db);
		}
		lock.clear();
	}

	database_ptr database_pool::connect(bool pooled_)
	{
		try
		{
			database_ptr db(new mysqlpp::connection(host.c_str(), userid.c_str(), passwd.c_str(), database.c_str(), port, pooled_));

			return db;
		}
		catch (...)
		{
			return database_ptr();
		}
	}

	bool database_pool::validate(database_ptr db)
	{
		try
		{
			std::auto_ptr<mysqlpp::statement> stmt(db->prepare("DO 0"));
			stmt->execute();

			return true;
		}
		catch (...)
		{

		}

		return false;
	}

} // namespace waspp
