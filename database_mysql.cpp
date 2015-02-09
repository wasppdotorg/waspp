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
		for (std::size_t i = 0; i < pool_size; ++i)
		{
			database_ptr db = connect_database();
			if (!validate_connection(db))
			{
				return false;
			}

			pool.push_back(db);
		}

		return true;
	}

	database_ptr database_mysql::acquire_connection()
	{
		boost::lock_guard<boost::mutex> lock(mutex_);

		if (pool.size() == 0)
		{
			return connect_database(false);
		}

		database_ptr db = *(pool.end() - 1);
		pool.pop_back();

		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration diff = now - db->released;

		if (diff.seconds() > wait_timeout && !validate_connection(db))
		{
			db.reset();
			return connect_database();
		}

		return db;
	}

	void database_mysql::release_connection(database_ptr db)
	{
		boost::lock_guard<boost::mutex> lock(mutex_);

		if (!db->pooled)
		{
			return;
		}

		db->released = boost::posix_time::second_clock::local_time();
		pool.push_back(db);
	}

	database_ptr database_mysql::connect_database(bool pooled)
	{
		try
		{
			sql::Driver *driver = get_driver_instance();

			database_ptr db;
			{
				db->conn = driver->connect("tcp://127.0.0.1:3306", "root", "1235");
				db->released = boost::posix_time::second_clock::local_time();
				db->pooled = pooled;
			}

			return db;
		}
		catch (sql::SQLException)
		{
			return database_ptr();
		}
	}

	bool database_mysql::validate_connection(database_ptr db)
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
