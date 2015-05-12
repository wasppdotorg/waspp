/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <mysql/mysql.h>

#include <stdexcept>
#include <vector>
#include <string>

#include <boost/unordered_map.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "database.hpp"
#include "config.hpp"
#include "dbconn_pool.hpp"

namespace waspp
{

	database::database() : shard_count(0)
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
			boost::unordered_map<std::string, std::string>& cfg_shard = cfg->get("shard");

			std::vector<std::string> keys;
			{
				keys.push_back("shard_count");
				keys.push_back("shard_format");
			}

			boost::unordered_map<std::string, std::string>::iterator found;
			for (std::size_t i = 0; i < keys.size(); ++i)
			{
				found = cfg_shard.find(keys[i]);
				if (found == cfg_shard.end())
				{
					return false;
				}

				if (keys[i] == "shard_count")
				{
					shard_count = boost::lexical_cast<unsigned int>(found->second);
				}
				else if (keys[i] == "shard_format")
				{
					shard_format = found->second;
				}
			}

			for (std::size_t i = 0; i < dbnames.size(); ++i)
			{
				dbpool_ptr dbpool(new dbconn_pool());

				if (!dbpool->init_pool(cfg->get(dbnames[i])) || !dbpool->fill_pool())
				{
					return false;
				}

				db_.insert(std::make_pair(dbnames[i], dbpool));
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
		boost::unordered_map<std::string, dbpool_ptr>::iterator found;
		found = db_.find(dbname);

		if (found == db_.end())
		{
			throw std::runtime_error("invalid dbname");
		}

		return found->second;
	}

	dbpool_ptr database::get_dbpool(unsigned long long int shard_key)
	{
		char format[8] = { 0 };

		int count = sprintf(format, shard_format.c_str(), shard_key % shard_count);
		if (count == 0)
		{
			throw std::runtime_error("invalid shard_format");
		}

		boost::unordered_map<std::string, dbpool_ptr>::iterator found;
		found = db_.find(std::string(format));

		if (found == db_.end())
		{
			throw std::runtime_error("invalid shard_key");
		}

		return found->second;
	}

	scoped_db::scoped_db(database* db_, const std::string& dbname_) : db(db_), dbname(dbname_), shard_key(0)
	{
	}

	scoped_db::scoped_db(database* db_, unsigned long long int shard_key_) : db(db_), dbname(std::string()), shard_key(shard_key_)
	{
	}

	scoped_db::~scoped_db()
	{
		if (!dbname.empty())
		{
			dbpool_ptr dbpool = db->get_dbpool(dbname);
			dbpool->free_dbconn(dbconn);
		}
		else if (shard_key != 0)
		{
			dbpool_ptr dbpool = db->get_dbpool(shard_key);
			dbpool->free_dbconn(dbconn);
		}
	}

	dbconn_ptr scoped_db::get()
	{
		try
		{
			if (!dbname.empty())
			{
				dbpool_ptr dbpool = db->get_dbpool(dbname);
				dbconn = dbpool->get_dbconn();
			}
			else if (shard_key != 0)
			{
				dbpool_ptr dbpool = db->get_dbpool(shard_key);
				dbconn = dbpool->get_dbconn();
			}
		}
		catch (...)
		{
			throw;
		}
		
		return dbconn;
	}

} // namespace waspp
