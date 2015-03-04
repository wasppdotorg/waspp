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

		/*
		template<typename T>
		dbconn_ptr get(T dbkey)
		{
			dbpool_ptr dbpool = get_dbpool(dbkey);
			return dbpool->get_dbconn();
		}
		*/

		dbconn_ptr get(const std::string& dbname)
		{
			dbpool_ptr dbpool = get_dbpool(dbname);
			return dbpool->get_dbconn();
		}

		dbconn_ptr get_shard(unsigned int shard_key)
		{
			dbpool_ptr dbpool = get_dbpool(shard_key);
			return dbpool->get_dbconn();
		}

		/*
		template<typename T>
		void free(T dbkey, dbconn_ptr dbconn)
		{
			dbpool_ptr dbpool = get_dbpool(dbkey);
			dbpool->free_dbconn(dbconn);
		}
		*/

		void free(const std::string& dbname, dbconn_ptr dbconn)
		{
			dbpool_ptr dbpool = get_dbpool(dbname);
			dbpool->free_dbconn(dbconn);
		}

		void free_shard(unsigned int shard_key, dbconn_ptr dbconn)
		{
			dbpool_ptr dbpool = get_dbpool(shard_key);
			dbpool->free_dbconn(dbconn);
		}

	private:
		dbpool_ptr get_dbpool(const std::string& dbname);
		dbpool_ptr get_dbpool(unsigned int shard_key);

		unsigned int shard_count;
		std::string shard_format;

		std::vector<dbpair> db_;

	};

} // namespace waspp

#endif // WASPP_DATABASE_HPP
