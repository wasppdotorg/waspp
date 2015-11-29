/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/property_tree/ptree.hpp>

// when using both boost asio and boost ptree on win32
// put these lines below before json_parser.hpp
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#include <boost/property_tree/json_parser.hpp>
#include <boost/lexical_cast.hpp>

#include "router.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "request.hpp"
#include "response.hpp"
#include "session.hpp"
#include "error.hpp"
#include "performance_checker.hpp"

namespace waspp
{
	namespace dir_performance
	{

		void test(config* cfg, request& req, response& res)
		{
			performance_checker c(50, __FILE__, __LINE__);

			error_type err_code = err_unknown;
			boost::property_tree::ptree json, error, session, forum_search, forum_item, forum_index, pagination;

			waspp::session sess(cfg, &req, &res);
			sess.put("userid", "test");

			err_code = err_none;

			error.put("_code", err_code);
			error.put("_message", cfg->err_msg(err_code));

			session.put("_userid", sess.get("userid"));
			session.put("_username", sess.get("username"));

			std::string field, keyword;
			if (req.rest_params.size() > 5)
			{
				field.append(req.rest_params[4]);
				keyword.append(req.rest_params[5]);
			}

			forum_search.put("_field", field);
			forum_search.put("_keyword", keyword);

			scoped_db db_etc("db_etc");

			long long int total_count_ = 0;
			stmt_ptr stmt(db_etc.prepare("SELECT COUNT(seq) AS total_count FROM forum"));

			rs_ptr rs(stmt->query());
			if (rs->fetch())
			{
				total_count_ = rs->get<long long int>("total_count");
			}

			stmt.reset(db_etc.prepare("SELECT * FROM forum ORDER BY seq DESC"));
			rs.reset(stmt->query());

			while (rs->fetch())
			{
				forum_item.put("_seq", rs->get<long long int>("seq"));
				forum_item.put("_title", rs->get<std::string>("title"));
				forum_item.put("_username", rs->get<std::string>("username"));
				forum_item.put("_updatetime", rs->get<std::string>("updatetime"));

				forum_index.push_back(std::make_pair("", forum_item));
			}

			pagination.put("_total_count", total_count_);
			pagination.put("_page_count", 10);
			pagination.put("_link_count", 10);

			json.put_child("_error", error);
			json.put_child("_session", session);
			json.put_child("_forum_search", forum_search);
			json.put_child("_forum_index", forum_index);
			json.put_child("_pagination", pagination);

			std::stringstream ss;
			write_json(ss, json, false);

			res.content.append(jsonp_start);
			res.content.append(ss.str());
			res.content.append(jsonp_end);
			res.content_extension = "js";
		}

	} // namespace dir_performance

} // namespace waspp
