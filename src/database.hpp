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

#include "singleton.hpp"
#include "dbconn_pool.hpp"

namespace waspp
{

	typedef boost::shared_ptr<dbconn_pool> dbcp_ptr;

	class database
		: public singleton<database>
	{
	public:
		database();
		~database();

		bool init(const std::vector<std::string>& dbkeys);

		/*
		template<typename T>
		dbconn_ptr get(T dbkey)
		{
			dbcp_ptr dbcp = find_dbcp(dbkey);
			return dbcp->get_dbconn();
		}
		*/

		dbconn_ptr get(const std::string& dbkey)
		{
			dbcp_ptr dbcp = find_dbcp(dbkey);
			return dbcp->get_dbconn();
		}

		dbconn_ptr get_shard(unsigned int shard_key)
		{
			dbcp_ptr dbcp = find_dbcp(shard_key);
			return dbcp->get_dbconn();
		}

		/*
		template<typename T>
		void free(T dbkey, dbconn_ptr dbconn)
		{
			dbcp_ptr dbcp = find_dbcp(dbkey);
			dbcp->free_dbconn(dbconn);
		}
		*/

		void free(const std::string& dbkey, dbconn_ptr dbconn)
		{
			dbcp_ptr dbcp = find_dbcp(dbkey);
			dbcp->free_dbconn(dbconn);
		}

		void free_shard(unsigned int shard_key, dbconn_ptr dbconn)
		{
			dbcp_ptr dbcp = find_dbcp(shard_key);
			dbcp->free_dbconn(dbconn);
		}

	private:
		dbcp_ptr find_dbcp(const std::string& dbkey);
		dbcp_ptr find_dbcp(unsigned int dbkey);

		unsigned int shard_count;
		std::string shard_format;

		std::map<std::string, dbcp_ptr> db_;

	};

} // namespace waspp

#endif // WASPP_DATABASE_HPP
