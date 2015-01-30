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

#include "database_mysql.hpp"

namespace waspp
{

bool database_myql::create_pool()
{
    for (std::size_t i = 0; i < pool_size; ++i)
    {
        database_ptr db = connect_database();
        if (!validate_connection(db))
        {
            return false;
        }

        pool.push_back(db);
    }

    return true;
}

database_ptr database_myql::acquire_connection()
{
    boost::lock_guard<boost::mutex> lock(mutex_);

    if (pool.size() == 0)
    {
        return connect_database(true);
    }

    database_ptr db = *(pool.end() - 1);
    pool.pop_back();

    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    boost::posix_time::time_duration diff = now - db->last_released;

    if (diff.seconds() > wait_timeout && !validate_connection(db))
    {
        db.reset();
        return connect_database();
    }

    return db;
}

void database_myql::release_connection(database_ptr db)
{
    boost::lock_guard<boost::mutex> lock(mutex_);

    if (db->connected_excessively)
    {
        return;
    }

    db->last_released = boost::posix_time::second_clock::local_time();
    pool.push_back(db);
}

database_ptr database_myql::connect_database(bool connect_excessively)
{
    try
    {
        sql::Driver *driver = get_driver_instance();

        database_ptr db;
        {
            db->conn = driver->connect("tcp://127.0.0.1:3306", "root", "1235");
            db->last_released = boost::posix_time::second_clock::local_time();
            db->connected_excessively = connect_excessively;
        }

        return db;
    }
    catch (sql::SQLException)
    {
        return database_ptr();
    }
}

bool database_myql::validate_connection(database_ptr db)
{
    try
    {
        std::auto_ptr<sql::Statement> stmt (db->conn->createStatement());
        stmt->executeQuery("DO 0");

        return true;
    }
    catch (sql::SQLException)
    {

    }

    return false;
}

} // namespace waspp
