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
#include "function_timer.hpp"

namespace waspp
{
	namespace dir_forum
	{
		void index_html(config* cfg, database* db, request& req, response& res)
		{
			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				res.redirect_to("/dir/user/signin");
				return;
			}

			router::get_file(cfg, res, "dir_include_header.html");
			router::get_file(cfg, res, "dir_forum_index.html");

			router::set_jsonp(req, res);
			router::get_file(cfg, res, "dir_include_footer.html");
		}

		void index_jsonp(config* cfg, database* db, request& req, response& res)
		{
			function_timer t(50, __FILE__, __LINE__);

			error_type err_code = err_unknown;
			boost::property_tree::ptree json, status, session, board_search, board_count, board_item, board_index, page_count, link_count;

			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				err_code = err_unauthorized;

				status.put("code", err_code);
				status.put("message", cfg->err_msg(err_code));

				json.put_child("status", status);

				std::stringstream ss;
				write_json(ss, json, false);

				res.content.append(jsonp_start);
				res.content.append(ss.str());
				res.content.append(jsonp_end);
				res.content_extension = "js";

				return;
			}

			err_code = err_none;

			status.put("code", err_code);
			status.put("message", cfg->err_msg(err_code));

			session.put("userid", sess.get("userid"));
			session.put("username", sess.get("username"));

			std::string field, keyword;
			if (req.rest_params.size() > 5)
			{
				field.append(req.rest_params[4]);
				keyword.append(req.rest_params[5]);
			}

			board_search.put("field", field);
			board_search.put("keyword", keyword);

			scoped_db db_etc_(db, "db_etc");
			dbconn_ptr db_etc = db_etc_.get();

			long long int board_count_ = 0;
			stmt_ptr stmt(db_etc->prepare("SELECT COUNT(seq) AS board_count FROM forum"));
				
			rs_ptr rs(stmt->query());
			if (rs->fetch())
			{
				board_count_ = rs->get<long long int>("board_count");
			}
			board_count.put("", board_count_);

			stmt.reset(db_etc->prepare("SELECT * FROM forum"));
			rs.reset(stmt->query());

			while (rs->fetch())
			{
				board_item.put("seq", rs->get<long long int>("seq"));
				board_item.put("title", rs->get<std::string>("title"));
				board_item.put("username", rs->get<std::string>("username"));
				board_item.put("updatetime", rs->get<std::string>("updatetime"));

				board_index.push_back(std::make_pair("", board_item));
			}

			page_count.put("", 10);
			link_count.put("", 10);

			json.put_child("status", status);
			json.put_child("session", session);
			json.put_child("board_search", board_search);
			json.put_child("board_index", board_index);
			json.put_child("board_count", board_count);
			json.put_child("page_count", page_count);
			json.put_child("link_count", link_count);

			std::stringstream ss;
			write_json(ss, json, false);

			res.content.append(jsonp_start);
			res.content.append(ss.str());
			res.content.append(jsonp_end);
			res.content_extension = "js";
		}

		void show_html(config* cfg, database* db, request& req, response& res)
		{
			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				res.redirect_to("/dir/user/signin");
				return;
			}

			router::get_file(cfg, res, "dir_include_header.html");
			router::get_file(cfg, res, "dir_forum_show.html");

			router::set_jsonp(req, res);
			router::get_file(cfg, res, "dir_include_footer.html");
		}

		void show_jsonp(config* cfg, database* db, request& req, response& res)
		{
			function_timer t(50, __FILE__, __LINE__);

			error_type err_code = err_unknown;
			boost::property_tree::ptree json, status, session, param, params;

			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				err_code = err_unauthorized;

				status.put("code", err_code);
				status.put("message", cfg->err_msg(err_code));

				json.put_child("status", status);

				std::stringstream ss;
				write_json(ss, json, false);

				res.content.append(jsonp_start);
				res.content.append(ss.str());
				res.content.append(jsonp_end);
				res.content_extension = "js";

				return;
			}

			err_code = err_none;

			status.put("code", err_code);
			status.put("message", cfg->err_msg(err_code));

			session.put("userid", sess.get("userid"));
			session.put("username", sess.get("username"));

			std::vector<std::string>::iterator i;
			for (i = req.rest_params.begin(); i != req.rest_params.end(); ++i)
			{
				param.put("", *i);
				params.push_back(std::make_pair("", param));
			}

			json.put_child("status", status);
			json.put_child("session", session);
			json.put_child("params", params);

			std::stringstream ss;
			write_json(ss, json, false);

			res.content.append(jsonp_start);
			res.content.append(ss.str());
			res.content.append(jsonp_end);
			res.content_extension = "js";
		}

		void form_html(config* cfg, database* db, request& req, response& res)
		{
			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				res.redirect_to("/dir/user/signin");
				return;
			}

			router::get_file(cfg, res, "dir_include_header.html");
			router::get_file(cfg, res, "dir_forum_form.html");

			router::set_jsonp(req, res);
			router::get_file(cfg, res, "dir_include_footer.html");
		}

		void form_jsonp(config* cfg, database* db, request& req, response& res)
		{
			function_timer t(50, __FILE__, __LINE__);

			error_type err_code = err_unknown;
			boost::property_tree::ptree json, status, session, param, params;

			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				err_code = err_unauthorized;

				status.put("code", err_code);
				status.put("message", cfg->err_msg(err_code));

				json.put_child("status", status);

				std::stringstream ss;
				write_json(ss, json, false);

				res.content.append(jsonp_start);
				res.content.append(ss.str());
				res.content.append(jsonp_end);
				res.content_extension = "js";

				return;
			}

			err_code = err_none;

			status.put("code", err_code);
			status.put("message", cfg->err_msg(err_code));

			session.put("userid", sess.get("userid"));
			session.put("username", sess.get("username"));

			std::vector<std::string>::iterator i;
			for (i = req.rest_params.begin(); i != req.rest_params.end(); ++i)
			{
				param.put("", *i);
				params.push_back(std::make_pair("", param));
			}

			json.put_child("status", status);
			json.put_child("session", session);
			json.put_child("params", params);

			std::stringstream ss;
			write_json(ss, json, false);

			res.content.append(jsonp_start);
			res.content.append(ss.str());
			res.content.append(jsonp_end);
			res.content_extension = "js";
		}

		void remove(config* cfg, database* db, request& req, response& res)
		{

		}

		void post(config* cfg, database* db, request& req, response& res)
		{
			function_timer t(50, __FILE__, __LINE__);

			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				res.redirect_to("/dir/user/signin");
				return;
			}

			unsigned int userid = boost::lexical_cast<unsigned int>(sess.get("userid"));
			unsigned int seq = 0;

			if (!req.param("seq").empty())
			{
				seq = boost::lexical_cast<unsigned int>(req.param("seq"));
			}

			if (req.param("title").empty())
			{
				router::err_msg(cfg, res, err_title_required);
				return;
			}

			if (req.param("content").empty())
			{
				router::err_msg(cfg, res, err_content_required);
				return;
			}

			scoped_db db_etc_(db, "db_etc");
			dbconn_ptr db_etc = db_etc_.get();

			if (seq == 0)
			{
				stmt_ptr stmt(db_etc->prepare("CALL USP_GET_UNIQUE_KEYS('forum', ?)"));
				{
					stmt->param(1);
				}

				rs_ptr rs(stmt->query());
				if (rs->fetch(true))
				{
					seq = rs->get<unsigned int>("last_key");
				}

				stmt.reset(db_etc->prepare("INSERT INTO forum(seq, title, content, file1, file2, userid, username, inserttime, updatetime) VALUES(?, ?, ?, ?, ?, ?, ?, NOW(), NOW())"));
				{
					stmt->param(seq);
					stmt->param(req.param("title"));
					stmt->param(req.param("content"));
					stmt->param("");
					stmt->param("");
					stmt->param(userid);
					stmt->param(sess.get("username"));
				}

				unsigned long long int affected_rows = stmt->execute();
				if (affected_rows == 0)
				{
					router::err_msg(cfg, res, err_forum_insert_failed);
					return;
				}
			}
			else
			{
				stmt_ptr stmt(db_etc->prepare("UPDATE forum SET title = ?, content = ?, file1 = ?, file2 = ?, username = ?, updatetime = NOW() WHERE seq = ? AND userid = ?"));
				{
					stmt->param(req.param("title"));
					stmt->param(req.param("content"));
					stmt->param("");
					stmt->param("");
					stmt->param(req.param("username"));
					stmt->param(seq);
					stmt->param(userid);
				}
				stmt->execute();
			}

			res.redirect_to("/dir/forum/index");
		}

	} // namespace dir_forum

} // namespace waspp
