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
	
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: ./waspp config.json server0\n";
			return 1;
		}

		if (!cfg->load(argv[1], argv[2]))
		{
			std::cerr << "config::load failed" << std::endl;
			return 1;
		}

		if (!log->config("debug", "minutely", "log.csv"))
		{
			std::cerr << "logger::config failed" << std::endl;
			return 1;
		}

		/*
		db_index.config(4, 3000);
		if (!db_index.fill_pool())
		{
			std::cerr << "database_pool::fill_pool failed" << std::endl;
			return 1;
		}
		*/

		waspp::server s(cfg->address, cfg->port, cfg->doc_root, cfg->num_threads);
		s.run();

		log->info("waspp started");
	}
	catch (std::exception& e)
	{
		log->fatal(e.what());
	}

	return 0;
}
