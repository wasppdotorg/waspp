//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

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

#include <boost/shared_ptr.hpp>

//#include "spinlock.hpp"
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

namespace waspp
{

	struct database
	{
		sql::Connection* conn;
		std::tm released;
		bool pooled;
	};

	typedef boost::shared_ptr<database> database_ptr;

	class database_pool
	{
	public:
		database_pool(std::size_t pool_size_, double wait_timeout_);
		~database_pool();

		bool fill_pool();

		database_ptr acquire_connection();
		void release_connection(database_ptr db);

	private:
		database_ptr connect(bool pooled = true);
		bool validate(database_ptr db);

		std::size_t pool_size;
		double wait_timeout;

		std::vector<database_ptr> pool;
		//spinlock lock;
		boost::detail::spinlock lock;

	};

} // namespace waspp

#endif // WASPP_DATABASE_POOL_HPP