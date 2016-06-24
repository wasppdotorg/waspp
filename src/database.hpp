/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef database_hpp
#define database_hpp

#include "dbconn_pool.hpp"
#include "singleton.hpp"
#include "config.hpp"

namespace waspp
{

	using dbpool_ptr = dbconn_pool*;

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

		std::unordered_map<std::string, dbpool_ptr> db_;

	};

	class scoped_db
	{
	public:
		scoped_db(const std::string& dbname);
		scoped_db(unsigned long long int shard_key);

		~scoped_db();

		dbconn_ptr ptr;

	private:
		dbpool_ptr dbpool;

	};

} // namespace waspp

#endif // database_hpp
