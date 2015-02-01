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

mysql_res::mysql_res(MYSQL_STMT* stmt)
{
	if (mysql_stmt_store_result(stmt) == 0)
	{
		throw mysql_runtime_error(mysql_stmt_error(stmt));
	}

	metadata = mysql_stmt_result_metadata(stmt);
	if (!metadata)
	{
		throw mysql_runtime_error("empty result");
	}

	field_count = mysql_stmt_field_count(stmt);
}

mysql_res::~mysql_res()
{
	if (metadata)
	{
		mysql_free_result(metadata);
	}
}

mysql_stmt::mysql_stmt(MYSQL* mysql, const std::string& query, const std::vector<mysql_param>& params)
{
	stmt = mysql_stmt_init(mysql);

	try
	{
		if (!stmt)
		{
			throw mysql_runtime_error("mysql_stmt_init failed");
		}

		if (mysql_stmt_prepare(stmt, query.c_str(), query.size()) != 0)
		{
			throw mysql_runtime_error(mysql_stmt_error(stmt));
		}

		param_count = mysql_stmt_param_count(stmt);
		if (param_count != params.size())
		{
			throw mysql_runtime_error("mysql_stmt_param_count not match");
		}

		if (!params.empty())
		{
			std::vector<MYSQL_BIND> binds;
			for (std::size_t i = 0; i < params.size(); ++i)
			{
				binds.push_back(params[i].bind);
			}

			if (mysql_stmt_bind_param(stmt, &binds[0]) != 0)
			{
				throw mysql_runtime_error(mysql_stmt_error(stmt));
			}
		}
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

	if (mysql_stmt_execute(stmt) != 0)
	{
		throw mysql_runtime_error(mysql_stmt_error(stmt));
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
