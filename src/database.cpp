/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <mysql/mysql.h>

#include <stdexcept>
#include <vector>
#include <string>
#include <unordered_map>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "database.hpp"
#include "config.hpp"
#include "dbconn_pool.hpp"
#include "mysqlpp.hpp"

namespace waspp
{

	database::database() : db_shard_count(0)
	{
		try
		{
			if (mysql_library_init(0, 0, 0) != 0)
			{
				throw std::runtime_error("mysql_library_init failed");
			}
		}
		catch (...)
		{
			mysql_library_end();
			throw;
		}
	}

	database::~database()
	{
		mysql_library_end();
	}

	bool database::init(config* cfg, const std::vector<std::string>& dbnames)
	{
		try
		{
			std::unordered_map<std::string, std::string>& cfg_db_shard = cfg->get("db_shard");

			std::vector<std::string> keys;
			{
				keys.push_back("db_shard_count");
				keys.push_back("db_shard_format");
			}

			for (auto& key : keys)
			{
				auto found = cfg_db_shard.find(key);
				if (found == cfg_db_shard.end())
				{
					return false;
				}

				if (key == "db_shard_count")
				{
					db_shard_count = boost::lexical_cast<unsigned int>(found->second);
				}
				else if (key == "db_shard_format")
				{
					db_shard_format = found->second;
				}
			}

			for (auto& dbname : dbnames)
			{
				dbpool_ptr dbpool(new dbconn_pool());

				if (!dbpool->init_pool(cfg->get(dbname)) || !dbpool->fill_pool())
				{
					return false;
				}

				db_.insert(std::make_pair(dbname, dbpool));
			}

			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

	dbpool_ptr database::get_dbpool(const std::string& dbname)
	{
		auto found = db_.find(dbname);
		if (found == db_.end())
		{
			throw std::runtime_error("invalid dbname");
		}

		return found->second;
	}

	dbpool_ptr database::get_dbpool(unsigned long long int shard_key)
	{
		char format[8] = { 0 };

		int count = sprintf(format, db_shard_format.c_str(), shard_key % db_shard_count);
		if (count == 0)
		{
			throw std::runtime_error("invalid db_shard_format");
		}

		auto found = db_.find(std::string(format));
		if (found == db_.end())
		{
			throw std::runtime_error("invalid db_shard_key");
		}

		return found->second;
	}

	scoped_db::scoped_db(const std::string& dbname)
	{
		dbpool = database::instance()->get_dbpool(dbname);
		ptr = dbpool->get_dbconn();
	}

	scoped_db::scoped_db(unsigned long long int shard_key)
	{
		dbpool = database::instance()->get_dbpool(shard_key);
		ptr = dbpool->get_dbconn();
	}

	scoped_db::~scoped_db()
	{
		dbpool->free_dbconn(ptr);
	}

} // namespace waspp
