//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_DATABASE_HPP
#define WASPP_DATABASE_HPP

#include <map>
#include <vector>
#include <string>

#include "utility.hpp"
#include "config.hpp"
#include "dbconn_pool.hpp"

namespace waspp
{

	typedef boost::shared_ptr<dbconn_pool> dbpool_ptr;

	class database
		: public utility::singleton<database>
	{
	public:
		database();
		~database();

		bool init(config* cfg, const std::vector<std::string>& dbkeys);

		/*
		template<typename T>
		dbconn_ptr get(T dbkey)
		{
			dbpool_ptr dbpool = get_dbpool(dbkey);
			return dbpool->get_dbconn();
		}
		*/

		dbconn_ptr get(const std::string& dbkey)
		{
			dbpool_ptr dbpool = get_dbpool(dbkey);
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

		void free(const std::string& dbkey, dbconn_ptr dbconn)
		{
			dbpool_ptr dbpool = get_dbpool(dbkey);
			dbpool->free_dbconn(dbconn);
		}

		void free_shard(unsigned int shard_key, dbconn_ptr dbconn)
		{
			dbpool_ptr dbpool = get_dbpool(shard_key);
			dbpool->free_dbconn(dbconn);
		}

	private:
		dbpool_ptr get_dbpool(const std::string& dbkey);
		dbpool_ptr get_dbpool(unsigned int dbkey);

		unsigned int shard_count;
		std::string shard_format;

		std::map<std::string, dbpool_ptr> db_;

	};

} // namespace waspp

#endif // WASPP_DATABASE_HPP
