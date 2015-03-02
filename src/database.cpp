/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <vector>
#include <string>

#include <boost/lexical_cast.hpp>

#include "config.hpp"
#include "database.hpp"
#include "dbconn_pool.hpp"

namespace waspp
{

	database::database() : shard_count(0)
	{
	}

	database::~database()
	{
	}

	bool database::init(config* cfg, const std::vector<std::string>& dbnames)
	{
		try
		{
			std::map<std::string, std::string>& cfg_shard = cfg->get("shard");

			std::vector<std::string> keys;
			{
				keys.push_back("shard_count");
				keys.push_back("shard_format");
			}

			for (std::size_t i = 0; i < keys.size(); ++i)
			{
				if (cfg_shard.find(keys[i]) == cfg_shard.end())
				{
					return false;
				}
			}

			shard_count = boost::lexical_cast<unsigned int>(cfg_shard.at("shard_count"));
			shard_format = cfg_shard.at("shard_format");

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
		std::map<std::string, dbpool_ptr>::iterator found;

		found = db_.find(dbname);
		if (found == db_.end())
		{
			throw std::runtime_error("invalid dbname");
		}

		return found->second;
	}

	dbpool_ptr database::get_dbpool(unsigned int shard_key)
	{
		char format[8] = {0};

		int count = sprintf(format, shard_format.c_str(), shard_key % shard_count);
		if (count == 0)
		{
			throw std::runtime_error("invalid shard_format");
		}

		std::map<std::string, dbpool_ptr>::iterator found;

		found = db_.find(std::string(format));
		if (found == db_.end())
		{
			throw std::runtime_error("invalid shard_key");
		}

		return found->second;
	}

} // namespace waspp
