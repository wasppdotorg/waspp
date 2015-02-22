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
#include "database.hpp"
#include "dbconn_pool.hpp"
#include "server.hpp"

int main(int argc, char* argv[])
{
	waspp::logger* log = waspp::logger::instance();
	waspp::config* cfg = waspp::config::instance();
	waspp::database* db = waspp::database::instance();

	try
	{
		if (0)
		//if (argc != 4)
		{
			std::cerr << "Usage: ./waspp log.csv config.json server0\n";
			return 1;
		}

		log->file("log.csv");
		//log->file(argv[1]);

		if (!cfg->init("config.json", "server0"))
		//if (!cfg->init(argv[2], argv[3]))
		{
			log->fatal("config::init failed");
			return 1;
		}

		if (!log->init(cfg->log_level, cfg->log_rotation))
		{
			log->fatal("logger::init failed");
			return 1;
		}

		std::vector<std::string> dbkeys;
		{
			dbkeys.push_back("db_index");
			dbkeys.push_back("db_000");
			dbkeys.push_back("db_001");
			dbkeys.push_back("db_etc");
		}

		if (!db->init(dbkeys))
		{
			log->fatal("database::init failed");
			return 1;
		}

		log->info("waspp starting..");
		
		waspp::server s(cfg->address, cfg->port, cfg->doc_root, cfg->num_threads);
		s.run();

		log->info("waspp stopped");
	}
	catch (std::exception& e)
	{
		log->fatal(e.what());
		return 1;
	}

	return 0;
}
