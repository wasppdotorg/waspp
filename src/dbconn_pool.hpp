/*
Copyright (c) 2014-2016 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_DBCONN_POOL_HPP
#define WASPP_DBCONN_POOL_HPP

#include <ctime>

#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "mysqlpp.hpp"
#include "utility.hpp"

namespace waspp
{

	typedef boost::shared_ptr<mysqlpp::connection> dbconn_ptr;
	typedef boost::scoped_ptr<mysqlpp::statement> stmt_ptr;
	typedef boost::scoped_ptr<mysqlpp::result> rs_ptr;

	class dbconn_pool
		: private boost::noncopyable
	{
	public:
		dbconn_pool();
		~dbconn_pool();

		bool init_pool(boost::unordered_map<std::string, std::string>& cfg);
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
