//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>
#include <string>

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

	bool database::init(const std::vector<std::string>& dbnames)
	{
		for (std::size_t i = 0; i < dbnames.size(); ++i)
		{
			dbconn_pool* dbcp = new dbconn_pool();
			db_.insert(std::make_pair(dbnames[i], dbcp));
		}

		config* cfg = config::instance();

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

	dbconn_ptr database::get(const std::string& dbname)
	{
		try
		{
			std::map<std::string, dbconn_pool*>::iterator found;

			found = db_.find(dbname);
			if (found == db_.end())
			{
				throw std::runtime_error("invalid dbname");
			}

			return db_.at(dbname)->get_dbconn();
		}
		catch (...)
		{
			throw;
		}

		return dbconn_ptr();
	}

	void database::free(const std::string& dbname, dbconn_ptr dbconn)
	{
		try
		{
			std::map<std::string, dbconn_pool*>::iterator found;

			found = db_.find(dbname);
			if (found == db_.end())
			{
				throw std::runtime_error("invalid dbname");
			}
		
			db_.at(dbname)->free_dbconn(dbconn);
		}
		catch (...)
		{
			throw;
		}
	}

} // namespace waspp
