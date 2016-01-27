/*
Copyright (c) 2014-2016 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_DATABASE_HPP
#define WASPP_DATABASE_HPP

#include <vector>
#include <string>

#include <boost/unordered_map.hpp>

#include "dbconn_pool.hpp"
#include "utility.hpp"
#include "config.hpp"
#include "mysqlpp.hpp"

namespace waspp
{

	typedef boost::shared_ptr<dbconn_pool> dbpool_ptr;

	class database
		: public singleton<database>
	{
	public:
		database();
		~database();

		bool init(config* cfg, const std::vector<std::string>& dbnames);

		dbpool_ptr get_dbpool(const std::string& dbname);
		dbpool_ptr get_dbpool(unsigned long long int shard_key);

	private:
		unsigned int db_shard_count;
		std::string db_shard_format;

		boost::unordered_map<std::string, dbpool_ptr> db_;

	};

	class scoped_db
	{
	public:
		scoped_db(const std::string& dbname);
		scoped_db(unsigned long long int shard_key);

		~scoped_db();

		mysqlpp::statement* prepare(const std::string& query);
		mysqlpp::statement* prepare_like(const std::string& query, bool left_percent, const std::string& keyword, bool right_percent);

	private:
		dbpool_ptr dbpool;
		dbconn_ptr dbconn;

	};

} // namespace waspp

#endif // WASPP_DATABASE_HPP
