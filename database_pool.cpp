//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "database_pool.hpp"

namespace waspp
{

	database_pool::database_pool() : pool_size(0), wait_timeout(0)
	{
	}

	database_pool::~database_pool()
	{
	}

	bool database_pool::create_pool()
	{
		lock.acquire();
		{
			for (std::size_t i = 0; i < pool_size; ++i)
			{
				database_ptr db = connect();
				if (!validate(db))
				{
					lock.release();
					return false;
				}

				pool.push_back(db);
			}
		}
		lock.release();

		return true;
	}

	database_ptr database_pool::acquire_connection()
	{
		database_ptr db;

		lock.acquire();
		{
			if (pool.empty())
			{
				lock.release();
				return connect();
			}

			db = *(pool.end() - 1);
		}
		lock.release();

		std::time_t now = std::time(0);
		double diff = std::difftime(now, mktime(&db->released));

		if (diff > wait_timeout && !validate(db))
		{
			return connect();
		}

		return db;
	}

	void database_pool::release_connection(database_ptr db)
	{
		if (!db->pooled)
		{
			return;
		}

		std::time_t now = std::time(0);
		db->released = *std::localtime(&now);

		lock.acquire();
		{
			pool.push_back(db);
		}
		lock.release();
	}

	database_ptr database_pool::connect(bool pooled)
	{
		try
		{
			sql::Driver *driver = get_driver_instance();
			std::time_t now = std::time(0);

			database_ptr db;
			{
				db->conn = driver->connect("tcp://127.0.0.1:3306", "root", "1235");
				db->released = *std::localtime(&now);
				db->pooled = pooled;
			}

			return db;
		}
		catch (sql::SQLException)
		{
			return database_ptr();
		}
	}

	bool database_pool::validate(database_ptr db)
	{
		try
		{
			std::auto_ptr<sql::Statement> stmt(db->conn->createStatement());
			stmt->executeQuery("DO 0");

			return true;
		}
		catch (sql::SQLException)
		{

		}

		return false;
	}

} // namespace waspp
