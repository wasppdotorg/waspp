/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef dbconn_pool_hpp
#define dbconn_pool_hpp

#include <memory>
#include <unordered_map>
#include <mutex>

#include "mysqlpp.hpp"

namespace waspp
{

	using dbconn_ptr = std::shared_ptr<mysqlpp::connection>;
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
		std::mutex mutex_;

	};

} // namespace waspp

#endif // dbconn_pool_hpp
