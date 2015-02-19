//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_DATABASE_POOL_HPP
#define WASPP_DATABASE_POOL_HPP

#include <ctime>

#include <vector>

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

#include "spinlock.hpp"
#include "mysqlpp.hpp"

namespace waspp
{

	typedef boost::shared_ptr<mysqlpp::connection> database_ptr;

	typedef boost::scoped_ptr<mysqlpp::statement> statement_ptr;
	typedef boost::scoped_ptr<mysqlpp::result> result_ptr;

	class database_pool
		: private boost::noncopyable
	{
	public:
		database_pool();
		~database_pool();

		bool init_pool(std::map<std::string, std::string> db_info_);
		bool fill_pool();

		database_ptr acquire_connection();
		void release_connection(database_ptr db);

	private:
		database_ptr connect(bool pooled_ = true);
		bool validate(database_ptr db);

		std::string host, userid, passwd, database;
		unsigned int port;

		std::size_t pool_size;
		double timeout_sec;

		std::vector<database_ptr> pool;
		spinlock lock;

	};

} // namespace waspp

#endif // WASPP_DATABASE_POOL_HPP
