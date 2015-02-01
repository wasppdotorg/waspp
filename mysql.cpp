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

#include <mysql/mysql.h>

#include "mysql.hpp"

namespace waspp
{

mysql_res::mysql_res(MYSQL_STMT* stmt_)
{
	field_count = mysql_stmt_field_count(stmt_);
	if (mysql_stmt_store_result(stmt_) == 0)
	{
		throw mysql_runtime_error(mysql_stmt_error(stmt_));
	}

	metadata = mysql_stmt_result_metadata(stmt_);
	if (!metadata)
	{
		throw mysql_runtime_error("empty result");
	}
}

mysql_res::~mysql_res()
{
	if (metadata)
	{
		mysql_free_result(metadata);
	}
}

mysql_stmt::mysql_stmt(MYSQL* mysql, const std::string& query_, const std::vector<mysql_param>& params_)
{
	stmt = mysql_stmt_init(mysql);

	try
	{
		if (!stmt)
		{
			throw mysql_runtime_error("mysql_stmt_init failed");
		}

		if (mysql_stmt_prepare(stmt, query_.c_str(), query_.size()) != 0)
		{
			throw mysql_runtime_error(mysql_stmt_error(stmt));
		}

		param_count = mysql_stmt_param_count(stmt);
	}
	catch (...)
	{
		if (stmt)
		{
			mysql_stmt_close(stmt);
		}

		throw;
	}
}

mysql_stmt::~mysql_stmt()
{
	mysql_stmt_close(stmt);
}

unsigned long long mysql_stmt::execute()
{
	//bind();

	if (mysql_stmt_execute(stmt) != 0 || mysql_stmt_store_result(stmt) != 0)
	{
		throw mysql_runtime_error(mysql_stmt_error(stmt));
	}

	MYSQL_RES* r = mysql_stmt_result_metadata(stmt);
	if (r)
	{
		mysql_free_result(r);
		throw mysql_runtime_error("execute_query to get result");
	}

	return mysql_stmt_affected_rows(stmt);
}

mysql_res* mysql_stmt::execute_query()
{
	//bind();

	if (mysql_stmt_execute(stmt) != 0)
	{
		throw mysql_runtime_error(mysql_stmt_error(stmt));
	}

	return new mysql_res(stmt);
}

mysql_conn::mysql_conn()
{
	mysql = mysql_init(0);
	if (!mysql)
	{
		throw mysql_runtime_error("mysql_init failed");
	}

	if (!mysql_real_connect(mysql, "127.0.0.1", "root", "1235", "test", 3306, 0, 0))
	{
		std::string mysql_conn_err = "unknown mysql connection error";

		try
		{
			mysql_conn_err = mysql_error(mysql);
		}
		catch (...)
		{
		}

		mysql_close(mysql);
		throw mysql_runtime_error(mysql_conn_err);
	}
}

mysql_conn::~mysql_conn()
{
	mysql_close(mysql);
}

mysql_stmt* mysql_conn::prepare_statement(const std::string& query)
{
	return new mysql_stmt(mysql, query, std::vector<mysql_param>());
}

mysql_stmt* mysql_conn::prepare_statement(const std::string& query, const std::vector<mysql_param>& params)
{
	return new mysql_stmt(mysql, query, params);
}

} // namespace waspp
