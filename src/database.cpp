/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/crc.hpp>

#include "database.hpp"
#include "locator.hpp"

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
		for (auto& d : db_)
		{
			delete d;
		}

		mysql_library_end();
	}

	bool database::init(const std::vector<std::string>& dbnames)
	{
		try
		{
			auto cfg = locator::cfg();
			auto cfg_db_shard = cfg->get("db_shard");

			std::vector<std::string> keys = { "db_shard_count", "db_shard_format" };
			for (auto& key : keys)
			{
				auto found = cfg_db_shard.find(key);
				if (found == cfg_db_shard.end())
				{
					return false;
				}

				if (key == "db_shard_count")
				{
					db_shard_count = strtoul(found->second.c_str(), nullptr, 0);
				}
				else if (key == "db_shard_format")
				{
					db_shard_format = found->second;
				}
			}

			for (auto& dbname : dbnames)
			{
				auto dbpool = new dbconn_pool();

				if (!dbpool->init_pool(cfg->get(dbname)) || !dbpool->fill_pool())
				{
					delete dbpool;
					return false;
				}

				db_.push_back(dbpool);
			}

			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

	dbconn_pool& database::get_dbpool(dbname_type dbname)
	{
		return *db_[(int)dbname];
	}

	dbconn_pool& database::get_dbpool(uint64_t shard_key)
	{
		return *db_[shard_key % db_shard_count];
	}

	dbconn_pool& database::get_dbpool(const std::string& shard_key)
	{
		boost::crc_32_type crc32;
		crc32.process_bytes(shard_key.c_str(), shard_key.size());

		return *db_[crc32.checksum() % db_shard_count];
	}

	scoped_db::scoped_db(dbname_type dbname)
		: dbpool(locator::db()->get_dbpool(dbname)),
		conn(dbpool.get_dbconn())
	{
		
	}

	scoped_db::scoped_db(uint64_t shard_key)
		: dbpool(locator::db()->get_dbpool(shard_key)),
		conn(dbpool.get_dbconn())
	{
		
	}

	scoped_db::scoped_db(const std::string& shard_key)
		: dbpool(locator::db()->get_dbpool(shard_key)),
		conn(dbpool.get_dbconn())
	{
		
	}

	scoped_db::~scoped_db()
	{
		dbpool.release_dbconn(conn);
	}

} // namespace waspp
