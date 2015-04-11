//
// Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>

#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"

#ifndef _WIN32
#include "redis.hpp"
#endif // _WIN32

#include "server.hpp"

int main(int argc, char* argv[])
{
	waspp::logger* log = waspp::logger::instance();
	waspp::config* cfg = waspp::config::instance();
	waspp::database* db = waspp::database::instance();
#ifndef _WIN32
	//waspp::redis* rd = waspp::redis::instance();
#endif // _WIN32

	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: ./waspp develop 00 &" << std::endl;
			//return 1;
		}

		std::string phase("develop"), server_seq("00");
		if (argc == 3)
		{
			phase = argv[1];
			server_seq = argv[2];
		}

		std::string log_file, cfg_file, server_id;
		{
			log_file.append("../log/");
			log_file.append(phase);
			log_file.append(server_seq);
			log_file.append(".csv");

			cfg_file.append("../cfg/");
			cfg_file.append(phase);
			cfg_file.append(".json");

			server_id.append("server");
			server_id.append(server_seq);
		}

		log->file(log_file);

		if (!cfg->init(cfg_file, server_id))
		{
			log->fatal(__FILE__, __LINE__, "config::init failed");
			return 1;
		}

		if (!log->init(cfg->level(), cfg->rotation()))
		{
			log->fatal(__FILE__, __LINE__, "logger::init failed");
			return 1;
		}

		std::vector<std::string> dbnames;
		{
			dbnames.push_back("db_idx");
			dbnames.push_back("db_000");
			dbnames.push_back("db_001");
			dbnames.push_back("db_etc");
		}

		if (!db->init(cfg, dbnames))
		{
			log->fatal(__FILE__, __LINE__, "database::init failed");
			return 1;
		}

#ifndef _WIN32
		std::vector<std::string> rdnames;
		{
			rdnames.push_back("rd_idx");
		}

		if (!rd->init(cfg, rdnames))
		{
			log->fatal(__FILE__, __LINE__, "redis::init failed");
			return 1;
		}
#endif // _WIN32

#ifndef CHECK_MEMORY_LEAK_WITH_SSL
		if (cfg->ssl())
		{
			log->fatal(__FILE__, __LINE__, "ssl disabled");
		}
		else
		{
			if (cfg->http2())
			{
				log->fatal(__FILE__, __LINE__, "enable ssl first");
			}
		}
#endif

		log->info("server starting..");
		waspp::server s(cfg);
		s.run();
	}
	catch (std::exception& e)
	{
		log->fatal(__FILE__, __LINE__, e.what());
		return 1;
	}

	return 0;
}
