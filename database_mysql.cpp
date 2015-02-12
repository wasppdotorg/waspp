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

#include "database_mysql.hpp"

namespace waspp
{

	database_mysql::database_mysql() : pool_size(0), wait_timeout(0), pool(0)
	{
	}

	database_mysql::~database_mysql()
	{
	}

	bool database_mysql::create_pool()
	{
		boost::lock_guard<boost::mutex> lock(mutex_);

		for (std::size_t i = 0; i < pool_size; ++i)
		{
			database_ptr db = connect();
			if (!validate(db))
			{
				return false;
			}

			pool.push_back(db);
		}

		return true;
	}

	database_ptr database_mysql::acquire_connection()
	{
		database_ptr db;
		if (!acquire(db))
		{
			return connect();
		}

		std::time_t now = std::time(0);
		double diff = difftime(now, mktime(&db->released));

		if (diff > wait_timeout && !validate(db))
		{
			return connect();
		}

		return db;
	}

	void database_mysql::release_connection(database_ptr db)
	{
		if (!db->pooled)
		{
			return;
		}

		std::time_t now = std::time(0);
		db->released = *std::localtime(&now);

		release(db);
	}

	database_ptr database_mysql::connect(bool pooled)
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

	bool database_mysql::validate(database_ptr db)
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

	bool database_mysql::acquire(database_ptr& db)
	{
		boost::lock_guard<boost::mutex> lock(mutex_);

		if (pool.empty())
		{
			return false;
		}

		db = *(pool.end() - 1);
		pool.pop_back();

		return true;
	}

	void database_mysql::release(database_ptr db)
	{
		boost::lock_guard<boost::mutex> lock(mutex_);

		pool.push_back(db);
	}

} // namespace waspp
