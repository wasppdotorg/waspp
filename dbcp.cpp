//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/lexical_cast.hpp>

#include "dbcp.hpp"

namespace waspp
{

	dbcp::dbcp()
		: port(0), pool_size(0), timeout_sec(0), pool(0)
	{
	}

	dbcp::~dbcp()
	{
	}

	bool dbcp::init_pool(const std::map<std::string, std::string>* c)
	{
		if (c == 0)
		{
			return false;
		}

		std::vector<std::string> keys;
		{
			keys.push_back("host");
			keys.push_back("userid");
			keys.push_back("passwd");
			keys.push_back("database");
			keys.push_back("port");
			keys.push_back("pool_size");
			keys.push_back("timeout_sec");
		}

		for (std::size_t i = 0; i < keys.size(); ++i)
		{
			if (c->find(keys[i]) == c->end())
			{
				return false;
			}
		}

		host = c->at("host");
		userid = c->at("userid");
		passwd = c->at("passwd");
		database = c->at("database");

		port = boost::lexical_cast<unsigned int>(c->at("port"));
		pool_size = boost::lexical_cast<std::size_t>(c->at("pool_size"));
		timeout_sec = boost::lexical_cast<double>(c->at("timeout_sec"));

		return true;
	}

	bool dbcp::fill_pool()
	{
		if (pool_size == 0)
		{
			return false;
		}

		lock.set();
		{
			for (std::size_t i = 0; i < pool_size; ++i)
			{
				db_conn_ptr db_conn = connect();
				if (!validate(db_conn))
				{
					lock.clear();
					return false;
				}

				pool.push_back(db_conn);
			}
		}
		lock.clear();

		return true;
	}

	db_conn_ptr dbcp::acquire_connection()
	{
		lock.set();
		//{
			if (pool.empty())
			{
				lock.clear();
				return connect(false);
			}

			db_conn_ptr db_conn = *(pool.end() - 1);
			pool.pop_back();
		//}
		lock.clear();

		std::time_t time_ = std::time(0);
		double diff = std::difftime(time_, mktime(db_conn->last_released()));

		if (diff > timeout_sec && !validate(db_conn))
		{
			return connect();
		}

		return db_conn;
	}

	void dbcp::release_connection(db_conn_ptr db_conn)
	{
		if (!db_conn->is_pooled())
		{
			return;
		}

		std::time_t time_ = std::time(0);
		db_conn->set_released(*std::localtime(&time_));

		lock.set();
		{
			pool.push_back(db_conn);
		}
		lock.clear();
	}

	db_conn_ptr dbcp::connect(bool pooled_)
	{
		return db_conn_ptr(new mysqlpp::connection(host.c_str(), userid.c_str(), passwd.c_str(), database.c_str(), port, pooled_));
	}

	bool dbcp::validate(db_conn_ptr db_conn)
	{
		try
		{
			boost::scoped_ptr<mysqlpp::statement> stmt(db_conn->prepare("DO 0"));
			stmt->execute();

			return true;
		}
		catch (...)
		{

		}

		return false;
	}

} // namespace waspp
