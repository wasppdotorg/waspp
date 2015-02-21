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
#include "dbcp.hpp"

namespace waspp
{

	database::database()
	{
	}

	database::~database()
	{
	}

	void database::add(const std::pair<std::string, dbcp*>& pair_)
	{
		db_.insert(pair_);
	}

	bool database::init()
	{
		config* c = config::instance();

		std::map<std::string, dbcp*>::iterator i;
		for (i = db_.begin(); i != db_.end(); ++i)
		{
			if (!i->second->init_pool(c->get(i->first)))
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

} // namespace waspp
