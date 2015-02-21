//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_DBCP_HPP
#define WASPP_DBCP_HPP

#include <ctime>

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "spinlock.hpp"
#include "mysqlpp.hpp"

namespace waspp
{
	typedef boost::shared_ptr<mysqlpp::connection> db_conn_ptr;

	typedef boost::scoped_ptr<mysqlpp::statement> statement_ptr;
	typedef boost::scoped_ptr<mysqlpp::result> result_ptr;

	class dbcp
		: private boost::noncopyable
	{
	public:
		dbcp();
		~dbcp();

		bool init_pool(const std::map<std::string, std::string>* c);
		bool fill_pool();

		db_conn_ptr acquire_connection();
		void release_connection(db_conn_ptr db_conn);

	private:
		db_conn_ptr connect(bool pooled_ = true);
		bool validate(db_conn_ptr db_conn);

		std::string host, userid, passwd, database;
		unsigned int port;

		std::size_t pool_size;
		double timeout_sec;

		std::vector<db_conn_ptr> pool;
		spinlock lock;

	};

} // namespace waspp

#endif // WASPP_DBCP_HPP
