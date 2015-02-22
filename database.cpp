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
		config* cfg = config::instance();

		std::map<std::string, std::string>* shard = cfg->get("shard");
		if (shard == 0)
		{
			return false;
		}

		std::vector<std::string> keys;
		{
			keys.push_back("shard_count");
			keys.push_back("shard_prefix");
			keys.push_back("shard_postfix");
		}

		for (std::size_t i = 0; i < keys.size(); ++i)
		{
			if (shard->find(keys[i]) == shard->end())
			{
				return false;
			}
		}

		shard_count = boost::lexical_cast<unsigned int>(shard->at("shard_count"));
		shard_prefix = shard->at("shard_prefix");
		shard_postfix = shard->at("shard_postfix");

		for (std::size_t i = 0; i < dbkeys.size(); ++i)
		{
			dbconn_pool* dbcp = new dbconn_pool();
			db_.insert(std::make_pair(dbkeys[i], dbcp));
		}

		std::map<std::string, dbconn_pool*>::iterator i;
		for (i = db_.begin(); i != db_.end(); ++i)
		{
			if (!i->second->init_pool(cfg->get(i->first)))
			{
				return false;
			}

			if (!i->second->fill_pool())
			{
				return false;
			}
		}
		
		return true;
	}

	dbconn_pool* database::this_db(const std::string& dbkey)
	{
		std::map<std::string, dbconn_pool*>::iterator found;

		found = db_.find(dbkey);
		if (found == db_.end())
		{
			throw std::runtime_error("invalid dbkey");
		}

		return found->second;
	}

	dbconn_pool* database::this_db(unsigned int dbkey)
	{
		char postfix[8] = {0};

		int count = sprintf(postfix, shard_postfix.c_str(), dbkey % shard_count);
		if (count == 0)
		{
			throw std::runtime_error("invalid dbkey");
		}

		std::map<std::string, dbconn_pool*>::iterator found;

		std::string dbkey_str(shard_prefix);
		dbkey_str.append(postfix);

		found = db_.find(dbkey_str);
		if (found == db_.end())
		{
			throw std::runtime_error("invalid dbkey");
		}

		return found->second;
	}

} // namespace waspp
