//
// Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "redis.hpp"
#include "locator.hpp"

#include "server.hpp"
#include "server_ssl.hpp"

// Winsock.h has already been included
#include <openssl/ssl.h>

void ssl_library_free()
{
	sk_SSL_COMP_free(SSL_COMP_get_compression_methods());
}

int main(int argc, char* argv[])
{
	redis3m::connection* rdconn = redis3m::connection::connect();
	rdconn->run(redis3m::command("SET") << "foo" << "bar");
	redis3m::reply r = rdconn->run(redis3m::command("GET") << "foo");
	std::cout << "foo is " << r.str() << "\n";
	delete rdconn;
	std::cin.get();
	return 0;
	atexit(ssl_library_free);

	waspp::logger log_;
	waspp::config cfg;
	waspp::database db;
	waspp::redis rd;

	waspp::locator::put(&log_);
	waspp::locator::put(&cfg);
	waspp::locator::put(&db);
	waspp::locator::put(&rd);

	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: ./waspp develop 0.0.0.0 &" << std::endl;
			//return 1;
		}

		std::string phase("develop"), address("0.0.0.0");
		if (argc == 3)
		{
			phase = argv[1];
			address = argv[2];
		}

		std::string log_file, cfg_file;
		//
			log_file.append("../log/");
			log_file.append(phase);
			log_file.append(".csv");

			cfg_file.append("../cfg/");
			cfg_file.append(phase);
			cfg_file.append(".json");
		//

		log_.file(log_file);

		if (!cfg.init(address, cfg_file))
		{
			waspp::log(waspp::fatal) << "config::init failed," << __FILE__ << ":" << __LINE__;
			return 1;
		}

		if (!log_.init())
		{
			waspp::log(waspp::fatal) << "logger::init failed," << __FILE__ << ":" << __LINE__;
			return 1;
		}

		if (!db.init({ "db_000", "db_001", "db_etc" }))
		{
			waspp::log(waspp::fatal) << "database::init failed," << __FILE__ << ":" << __LINE__;
			return 1;
		}

		if (!rd.init({ "rd_rnk" }))
		{
			waspp::log(waspp::fatal) << "redis::init failed," << __FILE__ << ":" << __LINE__;
			return 1;
		}

		if (cfg.ssl())
		{
			waspp::server_ssl s(cfg);
			waspp::log(waspp::info) << "server_ssl starting..";
			s.run();
		}
		else
		{
			waspp::server s(cfg);
			waspp::log(waspp::info) << "server starting..";
			s.run();
		}
	}
	catch (std::exception& e)
	{
		waspp::log(waspp::fatal) << e.what() << "," << __FILE__ << ":" << __LINE__;
	}

	return 0;
}
