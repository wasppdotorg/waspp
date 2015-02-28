//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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

	bool database::init(config* cfg, const std::vector<std::string>& dbkeys)
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

			for (std::size_t i = 0; i < dbkeys.size(); ++i)
			{
				dbpool_ptr dbpool(new dbconn_pool());

				if (!dbpool->init_pool(cfg->get(dbkeys[i])) || !dbpool->fill_pool())
				{
					return false;
				}

				db_.insert(std::make_pair(dbkeys[i], dbpool));
			}

			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

	dbpool_ptr database::get_dbpool(const std::string& dbkey)
	{
		std::map<std::string, dbpool_ptr>::iterator found;

		found = db_.find(dbkey);
		if (found == db_.end())
		{
			throw std::runtime_error("invalid dbkey");
		}

		return found->second;
	}

	dbpool_ptr database::get_dbpool(unsigned int dbkey)
	{
		char format[8] = {0};

		int count = sprintf(format, shard_format.c_str(), dbkey % shard_count);
		if (count == 0)
		{
			throw std::runtime_error("invalid dbkey");
		}

		std::map<std::string, dbpool_ptr>::iterator found;

		found = db_.find(std::string(format));
		if (found == db_.end())
		{
			throw std::runtime_error("invalid dbkey");
		}

		return found->second;
	}

} // namespace waspp
