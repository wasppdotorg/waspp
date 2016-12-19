/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef locator_hpp
#define locator_hpp

#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "redis.hpp"

namespace waspp
{

	class logger;
	class config;
	class database;
	class redis;

	class locator
	{
	public:
		static logger* log_() { return logger_; }
		static config* cfg() { return config_; }
		static database* db() { return database_; }
		static redis* rd() { return redis_; }

		static void put(logger* log_) { logger_ = log_; }
		static void put(config* cfg) { config_ = cfg; }
		static void put(database* db) { database_ = db; }
		static void put(redis* rd) { redis_ = rd; }
		
	private:
		static logger* logger_;
		static config* config_;
		static database* database_;
		static redis* redis_;

	};

} // namespace waspp

#endif // locator_hpp
