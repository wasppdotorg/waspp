/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_DBCONN_POOL_HPP
#define WASPP_DBCONN_POOL_HPP

#include <ctime>

#include <vector>
#include <unordered_map>

#include "mysqlpp.hpp"
#include "utility.hpp"

namespace waspp
{

	using dbconn_ptr = mysqlpp::connection*;
	using stmt_ptr = std::unique_ptr<mysqlpp::statement>;
	using rs_ptr = std::unique_ptr<mysqlpp::result>;

	class dbconn_pool
	{
	public:
		dbconn_pool(const dbconn_pool&) = delete;
		dbconn_pool& operator=(const dbconn_pool&) = delete;

		dbconn_pool();
		~dbconn_pool();

		bool init_pool(std::unordered_map<std::string, std::string>& cfg);
		bool fill_pool();

		dbconn_ptr get_dbconn();
		void free_dbconn(dbconn_ptr dbconn);

	private:
		dbconn_ptr connect(bool pooled_ = true);

		std::string host, userid, passwd, database;
		unsigned int port;
		std::string charset;

		std::size_t pool_size;
		double wait_timeout_sec;

		std::vector<dbconn_ptr> pool;
		spinlock lock;

	};

} // namespace waspp

#endif // WASPP_DBCONN_POOL_HPP
