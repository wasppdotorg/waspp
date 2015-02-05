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

#ifndef WASPP_DATABASE_MYSQL_HPP
#define WASPP_DATABASE_MYSQL_HPP

#include <vector>

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

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
    boost::posix_time::ptime last_released;
    bool connected_excessively;
};

typedef boost::shared_ptr<database> database_ptr;

class database_mysql
{
public:
	database_mysql();
	~database_mysql();

    bool create_pool();

    database_ptr acquire_connection();
    void release_connection(database_ptr db);

private:
    database_ptr connect_database(bool connect_excessively = false);
    bool validate_connection(database_ptr db);

    std::size_t pool_size;
    int64_t wait_timeout;

    std::vector<database_ptr> pool;
    boost::mutex mutex_;
};

} // namespace waspp

#endif // WASPP_DATABASE_MYSQL_HPP
