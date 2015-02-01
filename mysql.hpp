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

#include <mysql/mysql.h>

#include "key_value.hpp"

namespace waspp
{

class mysql_runtime_error : public std::runtime_error
{
public:
	mysql_runtime_error(const std::string& what_arg) : std::runtime_error(what_arg)
	{
	}
};

struct mysql_param
{
	mysql_param() : is_null(true), is_blob(false), length(0), buffer(0)
	{
	}

	bool is_null;
	bool is_blob;

	std::string value;
	unsigned long length;
	
	void *buffer;
};

struct bind_data
{
	bind_data() : ptr(0), length(0), is_null(false), error(false)
	{
	}

	char buf[128];
	std::vector<char> vbuf;
	char* ptr;
	unsigned long length;
	bool is_null;
	bool error;
};

class mysql_res
{
public:
	mysql_res(MYSQL_STMT* stmt_);
	~mysql_res();

private:
	int field_count;
	unsigned int current_row;

	MYSQL_RES* metadata;
	std::vector<MYSQL_BIND> binds;
	std::vector<bind_data> bind_datas;
};

class mysql_stmt
{
public:
	mysql_stmt(MYSQL* mysql_, const std::string& query_, const std::vector<mysql_param>& params_);
	~mysql_stmt();

	//void bind();
	unsigned long long execute();
	mysql_res* execute_query();

private:
	MYSQL_STMT* stmt;
	int param_count;

	std::vector<MYSQL_BIND> binds;
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
