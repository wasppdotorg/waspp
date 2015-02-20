//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "config.hpp"
#include "logger.hpp"
#include "database_pool.hpp"
#include "server.hpp"

int main(int argc, char* argv[])
{
	waspp::config* cfg = waspp::config::instance();
	waspp::logger* log = waspp::logger::instance();

	std::map<std::string, waspp::database_pool*> db_pools;

	try
	{
		//argc = 3;
		//argv[1] = "config.json";
		//argv[2] = "server0";

		if (argc != 3)
		{
			std::cerr << "Usage: ./waspp config.json server0\n";
			return 1;
		}

		if (!cfg->init(argv[1], argv[2]))
		{
			std::cerr << "config::init failed" << std::endl;
			return 1;
		}

		if (!log->init(cfg->log_level, cfg->log_rotation, cfg->log_file))
		{
			std::cerr << "logger::init failed" << std::endl;
			return 1;
		}

		waspp::database_pool db_index, db_000, db_001, db_etc;
		{
			db_pools.insert(std::make_pair("db_index", &db_index));
			db_pools.insert(std::make_pair("db_000", &db_000));
			db_pools.insert(std::make_pair("db_001", &db_001));
			db_pools.insert(std::make_pair("db_etc", &db_etc));
		}

		std::map<std::string, waspp::database_pool*>::iterator i;
		for (i = db_pools.begin(); i != db_pools.end(); ++i)
		{
			if (!i->second->init_pool(cfg->get(i->first)))
			{
				std::cerr << "database_pool::init_pool failed" << std::endl;
				return 1;
			}

			if (!i->second->fill_pool())
			{
				std::cerr << "database_pool::fill_pool failed" << std::endl;
				return 1;
			}
		}

		waspp::server s(cfg->address, cfg->port, cfg->doc_root, cfg->num_threads, db_pools);
		s.run();

		log->info("waspp started");
	}
	catch (std::exception& e)
	{
		log->fatal(e.what());
	}

	return 0;
}
