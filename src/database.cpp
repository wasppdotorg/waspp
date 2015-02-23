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

	database::database()
	{
	}

	database::~database()
	{
	}

	bool database::init(const std::vector<std::string>& dbkeys)
	{
		try
		{
			config* cfg = config::instance();

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

			shard_count = boost::lexical_cast<unsigned int>(cfg_shard["shard_count"]);
			shard_format = cfg_shard["shard_format"];

			for (std::size_t i = 0; i < dbkeys.size(); ++i)
			{
				dbconn_pool* dbcp = new dbconn_pool();

				if (!dbcp->init_pool(cfg->get(dbkeys[i])) || !dbcp->fill_pool())
				{
					return false;
				}

				db_.insert(std::make_pair(dbkeys[i], dbcp));
			}

			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

	dbconn_pool& database::this_db(const std::string& dbkey)
	{
		std::map<std::string, dbconn_pool*>::iterator found;

		found = db_.find(dbkey);
		if (found == db_.end())
		{
			throw std::runtime_error("invalid dbkey");
		}

		return *(found->second);
	}

	dbconn_pool& database::this_db(unsigned int dbkey)
	{
		char format[8] = {0};

		int count = sprintf(format, shard_format.c_str(), dbkey % shard_count);
		if (count == 0)
		{
			throw std::runtime_error("invalid dbkey");
		}

		std::map<std::string, dbconn_pool*>::iterator found;

		found = db_.find(std::string(format));
		if (found == db_.end())
		{
			throw std::runtime_error("invalid dbkey");
		}

		return *(found->second);
	}

} // namespace waspp
