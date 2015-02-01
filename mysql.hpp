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

#ifndef WASPP_MYSQL_HPP
#define WASPP_MYSQL_HPP

#include <vector>
#include <stdexcept>

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

#include <mysql/mysql.h>

namespace waspp
{

/*
+---------------+----------------------+-------------------------+
| c_types       | buffer_types         | sql_types               |
+---------------+----------------------+-------------------------+
| signed char   | MYSQL_TYPE_TINY      | TINYINT                 |
| short int     | MYSQL_TYPE_SHORT     | SMALLINT                |
| int           | MYSQL_TYPE_LONG      | INT                     |
| long long int | MYSQL_TYPE_LONGLONG  | BIGINT                  |
| float         | MYSQL_TYPE_FLOAT     | FLOAT                   |
| double        | MYSQL_TYPE_DOUBLE    | DOUBLE                  |
| MYSQL_TIME    | MYSQL_TYPE_TIME      | TIME                    |
| MYSQL_TIME    | MYSQL_TYPE_DATE      | DATE                    |
| MYSQL_TIME    | MYSQL_TYPE_DATETIME  | DATETIME                |
| MYSQL_TIME    | MYSQL_TYPE_TIMESTAMP | TIMESTAMP               |
| char[]        | MYSQL_TYPE_STRING    | TEXT, CHAR, VARCHAR     |
| char[]        | MYSQL_TYPE_BLOB      | BLOB, BINARY, VARBINARY |
|               | MYSQL_TYPE_NULL      | NULL                    |
+---------------+----------------------+-------------------------+
*/

class mysql_runtime_error : public std::runtime_error
{
public:
	mysql_runtime_error(const std::string& what_arg) : std::runtime_error(what_arg)
	{
		
	}
};

struct mysql_param
{
	mysql_param(const std::string& value)
	{
		make_bind(value, false, false);
	}

	mysql_param(const std::string& value, bool is_null)
	{
		make_bind(value, is_null, false);
	}

	mysql_param(const std::string& value, bool is_null, bool is_blob)
	{
		make_bind(value, is_null, is_blob);
	}

	void make_bind(const std::string& value, bool is_null, bool is_blob)
	{
		if (is_null)
		{
			bind.buffer_type = MYSQL_TYPE_NULL;
			return;
		}

		bind.buffer_type = MYSQL_TYPE_STRING;
		if (is_blob)
		{
			bind.buffer_type = MYSQL_TYPE_BLOB;
		}

		bind.buffer = const_cast<char*>(value.c_str());
		bind.buffer_length = value.size();

		unsigned long length = boost::lexical_cast<unsigned long>(value.size());
		bind.length = &length;
	}
	
	MYSQL_BIND bind;
};

class mysql_res
{
public:
	mysql_res(MYSQL_STMT* stmt);
	~mysql_res();

private:
	unsigned int field_count;
	unsigned int current_row;

	MYSQL_RES* metadata;
	std::vector<mysql_param> params;
};

class mysql_stmt
{
public:
	mysql_stmt(MYSQL* mysql, const std::string& query, const std::vector<mysql_param>& params);
	~mysql_stmt();

	//void bind();
	unsigned long long execute();
	mysql_res* execute_query();

private:
	MYSQL_STMT* stmt;
	int param_count;

	std::vector<mysql_param> params;
};

class mysql_conn
{
public:
	mysql_conn();
	~mysql_conn();

	mysql_stmt* prepare_statement(const std::string& query);
	mysql_stmt* prepare_statement(const std::string& query, const std::vector<mysql_param>& params);

private:
	MYSQL* mysql;
};

} // namespace waspp

#endif // WASPP_MYSQL_HPP
