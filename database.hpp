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

	class database
		: public singleton<database>
	{
	public:
		database();
		~database();

		bool init(const std::vector<std::string>& dbkeys);
		
		template<typename T>
		dbconn_ptr get(T dbkey)
		{
			dbconn_pool* db = this_db(dbkey);
			return db->get_dbconn();
		}
		
		template<typename T>
		void free(T dbkey, dbconn_ptr dbconn)
		{
			dbconn_pool* db = this_db(dbkey);
			db->free_dbconn(dbconn);
		}
		
	private:
		dbconn_pool* this_db(const std::string& dbkey);
		dbconn_pool* this_db(unsigned int dbkey);

		unsigned int shard_count;
		std::string shard_prefix;
		std::string shard_postfix;

		std::map<std::string, dbconn_pool*> db_;

	};

} // namespace waspp

#endif // WASPP_DATABASE_HPP
