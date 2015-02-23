//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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

	bool dbconn_pool::init_pool(const std::map<std::string, std::string>& cfg)
	{
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
			if (cfg.find(keys[i]) == cfg.end())
			{
				return false;
			}
		}

		host = cfg.at("host");
		userid = cfg.at("userid");
		passwd = cfg.at("passwd");
		database = cfg.at("database");

		port = boost::lexical_cast<unsigned int>(cfg.at("port"));
		pool_size = boost::lexical_cast<std::size_t>(cfg.at("pool_size"));
		timeout_sec = boost::lexical_cast<double>(cfg.at("timeout_sec"));

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

		std::time_t time_ = std::time(0);
		double diff = std::difftime(time_, mktime(dbconn->last_released()));

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
		return dbconn_ptr(new mysqlpp::connection(host.c_str(), userid.c_str(), passwd.c_str(), database.c_str(), port, pooled_));
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

		}

		return false;
	}

} // namespace waspp
