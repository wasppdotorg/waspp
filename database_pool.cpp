//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "database_pool.hpp"

namespace waspp
{

	database_pool::database_pool(std::size_t pool_size_, double wait_timeout_)
		: pool_size(pool_size_), wait_timeout(wait_timeout_), pool(0)
	{
	}

	database_pool::~database_pool()
	{
	}

	bool database_pool::fill_pool()
	{
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

		if (diff > wait_timeout && !validate(db))
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
			database_ptr db(new mysqlpp::connection("127.0.0.1", "root", "1235", "test"));

			std::time_t time_ = std::time(0);
			db->set_released(*std::localtime(&time_));

			db->set_pooled(pooled_);

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
