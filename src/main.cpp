//
// Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <sstream>
#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "server.hpp"

int main(int argc, char* argv[])
{
	waspp::logger* log = waspp::logger::instance();
	waspp::config* cfg = waspp::config::instance();
	waspp::database* db = waspp::database::instance();

	try
	{
		//if (0)
		if (argc != 3)
		{
			std::cerr << "Usage: ./waspp develop 00 &\n";
			return 1;
		}

		std::ostringstream oss;
		std::string log_file, cfg_file, server_id;
		//if (0)
		{
			oss.str(std::string());
			oss << "../log/" << argv[1] << argv[2] << ".csv";
			log_file.append(oss.str());

			oss.str(std::string());
			oss << "../cfg/" << argv[1] << ".json";
			cfg_file.append(oss.str());

			oss.str(std::string());
			oss << "server" << argv[2];
			server_id.append(oss.str());
		}
		//log_file.append("../log/develop.csv");
		//cfg_file.append("../cfg/develop.json");
		//server_id.append("server00");

		log->file(log_file);

		if (!cfg->init(cfg_file, server_id))
		{
			log->fatal("config::init failed");
			return 1;
		}

		if (!log->init(cfg->level, cfg->rotation))
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

		if (!db->init(cfg, dbkeys))
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
