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
	waspp::config* cfg = waspp::config::instance();
	waspp::logger* log = waspp::logger::instance();
	waspp::database* db = waspp::database::instance();

	try
	{
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

		std::vector<std::string> dbnames;
		{
			dbnames.push_back("db_index");
			dbnames.push_back("db_000");
			dbnames.push_back("db_001");
			dbnames.push_back("db_etc");
		}

		if (!db->init(dbnames))
		{
			std::cerr << "database::init failed" << std::endl;
			return 1;
		}

		log->info("waspp starting..");
		waspp::server s(cfg->address, cfg->port, cfg->doc_root, cfg->num_threads);
		s.run();
	}
	catch (std::exception& e)
	{
		log->fatal(e.what());
	}

	return 0;
}
