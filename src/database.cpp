/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <vector>
#include <string>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "database.hpp"
#include "config.hpp"
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
			std::vector<name_value>& cfg_shard = cfg->get("shard");

			std::vector<std::string> keys;
			{
				keys.push_back("shard_count");
				keys.push_back("shard_format");
			}

			std::vector<name_value>::iterator found;
			for (std::size_t i = 0; i < keys.size(); ++i)
			{
				found = std::find_if(cfg_shard.begin(), cfg_shard.end(), boost::bind(&name_value::compare_name, _1, keys[i]));
				if (found == cfg_shard.end())
				{
					return false;
				}

				if (keys[i] == "shard_count")
				{
					shard_count = boost::lexical_cast<unsigned int>(found->value);
				}
				else if (keys[i] == "shard_format")
				{
					shard_format = found->value;
				}
			}

			for (std::size_t i = 0; i < dbnames.size(); ++i)
			{
				dbpool_ptr dbpool(new dbconn_pool());

				if (!dbpool->init_pool(cfg->get(dbnames[i])) || !dbpool->fill_pool())
				{
					return false;
				}

				db_.push_back(dbpair(dbnames[i], dbpool));
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
		std::vector<dbpair>::iterator found;
		found = std::find_if(db_.begin(), db_.end(), boost::bind(&dbpair::compare_first, _1, dbname));

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

		std::vector<dbpair>::iterator found;
		found = std::find_if(db_.begin(), db_.end(), boost::bind(&dbpair::compare_first, _1, std::string(format)));

		if (found == db_.end())
		{
			throw std::runtime_error("invalid shard_key");
		}

		return found->second;
	}

} // namespace waspp
