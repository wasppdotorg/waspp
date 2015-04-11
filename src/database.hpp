/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_DATABASE_HPP
#define WASPP_DATABASE_HPP

#include <vector>
#include <string>

#include "dbconn_pool.hpp"
#include "utility.hpp"
#include "config.hpp"

namespace waspp
{

	typedef boost::shared_ptr<dbconn_pool> dbpool_ptr;

	struct dbpair
	{
		dbpair(const std::string& first_, dbpool_ptr second_) : first(first_), second(second_)
		{
		}

		bool compare_first(const std::string& first_)
		{
			return first == first_;
		}

		std::string first;
		dbpool_ptr second;
	};

	class database
		: public singleton<database>
	{
	public:
		database();
		~database();

		bool init(config* cfg, const std::vector<std::string>& dbnames);

		dbpool_ptr get_dbpool(const std::string& dbname);
		dbpool_ptr get_dbpool(unsigned int shard_key);

	private:
		unsigned int shard_count;
		std::string shard_format;

		std::vector<dbpair> db_;

	};

	class database_guard
	{
	public:
		database_guard(database* db_, const std::string& dbname_);
		database_guard(database* db_, unsigned int shard_key_);
		
		~database_guard();

		dbconn_ptr get();
		
	private:
		database* db;

		std::string dbname;
		unsigned int shard_key;

		dbconn_ptr dbconn;

	};

} // namespace waspp

#endif // WASPP_DATABASE_HPP
