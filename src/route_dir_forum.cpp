/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

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
#include "database.hpp"
#include "session.hpp"
#include "performance_checker.hpp"

namespace waspp
{
	
	namespace dir_forum
	{

		void index_html(config* cfg, request& req, response& res)
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

		void index_jsonp(config* cfg, request& req, response& res)
		{
			performance_checker c(50, __FILE__, __LINE__);

			error_type err_code = err_unknown;
			boost::property_tree::ptree json, error, session, forum_search, forum_item, forum_index, pagination;

			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				err_code = err_unauthorized;

				error.put("_code", err_code);
				error.put("_message", cfg->err_msg(err_code));

				json.put_child("_error", error);

				std::stringstream ss;
				write_json(ss, json, false);

				res.content.append(jsonp_start);
				res.content.append(ss.str());
				res.content.append(jsonp_end);
				res.content_extension = "js";

				return;
			}

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
			stmt_ptr stmt(db_etc.ptr->prepare("SELECT COUNT(seq) AS total_count FROM forum"));

			rs_ptr rs(stmt->query());
			if (rs->fetch())
			{
				total_count_ = rs->get<long long int>("total_count");
			}

			//long long int per_page = 10;
			stmt.reset(db_etc.ptr->prepare("SELECT * FROM forum ORDER BY seq DESC"));
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

		void show_html(config* cfg, request& req, response& res)
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

		void show_jsonp(config* cfg, request& req, response& res)
		{
			performance_checker c(50, __FILE__, __LINE__);

			error_type err_code = err_unknown;
			boost::property_tree::ptree json, error, session, param, params;

			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				err_code = err_unauthorized;

				error.put("_code", err_code);
				error.put("_message", cfg->err_msg(err_code));

				json.put_child("_error", error);

				std::stringstream ss;
				write_json(ss, json, false);

				res.content.append(jsonp_start);
				res.content.append(ss.str());
				res.content.append(jsonp_end);
				res.content_extension = "js";

				return;
			}

			err_code = err_none;

			error.put("_code", err_code);
			error.put("_message", cfg->err_msg(err_code));

			session.put("_userid", sess.get("userid"));
			session.put("_username", sess.get("username"));

			//std::vector<std::string>::iterator i;
			for (auto& rest_param : req.rest_params)
			{
				param.put("", rest_param);
				params.push_back(std::make_pair("", param));
			}

			json.put_child("_error", error);
			json.put_child("_session", session);
			json.put_child("_params", params);

			std::stringstream ss;
			write_json(ss, json, false);

			res.content.append(jsonp_start);
			res.content.append(ss.str());
			res.content.append(jsonp_end);
			res.content_extension = "js";
		}

		void form_html(config* cfg, request& req, response& res)
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

		void form_jsonp(config* cfg, request& req, response& res)
		{
			performance_checker c(50, __FILE__, __LINE__);

			error_type err_code = err_unknown;
			boost::property_tree::ptree json, error, session, param, params;

			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				err_code = err_unauthorized;

				error.put("_code", err_code);
				error.put("_message", cfg->err_msg(err_code));

				json.put_child("_error", error);

				std::stringstream ss;
				write_json(ss, json, false);

				res.content.append(jsonp_start);
				res.content.append(ss.str());
				res.content.append(jsonp_end);
				res.content_extension = "js";

				return;
			}

			err_code = err_none;

			error.put("_code", err_code);
			error.put("_message", cfg->err_msg(err_code));

			session.put("_userid", sess.get("userid"));
			session.put("_username", sess.get("username"));

			//std::vector<std::string>::iterator i;
			for (auto& rest_param : req.rest_params)
			{
				param.put("", rest_param);
				params.push_back(std::make_pair("", param));
			}

			json.put_child("_error", error);
			json.put_child("_session", session);
			json.put_child("_params", params);

			std::stringstream ss;
			write_json(ss, json, false);

			res.content.append(jsonp_start);
			res.content.append(ss.str());
			res.content.append(jsonp_end);
			res.content_extension = "js";
		}

		void del(config* cfg, request& req, response& res)
		{

		}

		void post(config* cfg, request& req, response& res)
		{
			performance_checker c(50, __FILE__, __LINE__);

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

			scoped_db db_etc("db_etc");

			if (seq == 0)
			{
				stmt_ptr stmt(db_etc.ptr->prepare("CALL USP_GET_UNIQUE_KEYS('forum', ?)"));
				//
					stmt->param(1);
				//

				rs_ptr rs(stmt->query());
				if (rs->fetch(true))
				{
					seq = rs->get<unsigned int>("last_key");
				}

				stmt.reset(db_etc.ptr->prepare("INSERT INTO forum(seq, title, content, file1, file2, userid, username, inserttime, updatetime) VALUES(?, ?, ?, ?, ?, ?, ?, NOW(), NOW())"));
				//
					stmt->param(seq);
					stmt->param(req.param("title"));
					stmt->param(req.param("content"));
					stmt->param("");
					stmt->param("");
					stmt->param(userid);
					stmt->param(sess.get("username"));
				//

				unsigned long long int affected_rows = stmt->execute();
				if (affected_rows == 0)
				{
					router::err_msg(cfg, res, err_forum_insert_failed);
					return;
				}
			}
			else
			{
				stmt_ptr stmt(db_etc.ptr->prepare("UPDATE forum SET title = ?, content = ?, file1 = ?, file2 = ?, username = ?, updatetime = NOW() WHERE seq = ? AND userid = ?"));
				//
					stmt->param(req.param("title"));
					stmt->param(req.param("content"));
					stmt->param("");
					stmt->param("");
					stmt->param(req.param("username"));
					stmt->param(seq);
					stmt->param(userid);
				//
				stmt->execute();
			}

			res.redirect_to("/dir/forum/index");
		}

	} // namespace dir_forum

} // namespace waspp
