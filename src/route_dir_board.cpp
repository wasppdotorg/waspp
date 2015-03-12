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
#include "jsonp.hpp"

namespace waspp
{
	namespace dir_board
	{
		void index_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				res.redirect_to("/dir/user/signin");
				return;
			}

			std::string full_path(cfg->doc_root + "/dir_board_index.html");
			router::get_file(res, full_path);
			router::put_jsonp(req, res);
		}

		void index_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{
			jsonp_status_type status_code = status_error;
			boost::property_tree::ptree json, status, session, param, params;

			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				status_code = status_unauthorized;

				status.put("code", status_code);
				status.put("message", jsonp_status::str(status_code));

				json.put_child("status", status);

				std::stringstream ss;
				write_json(ss, json, false);

				res.content.append(jsonp_start);
				res.content.append(ss.str());
				res.content.append(jsonp_end);
				res.content_extension = "js";

				return;
			}

			status_code = status_okay;

			status.put("code", status_code);
			status.put("message", jsonp_status::str(status_code));

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

		void show_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				res.redirect_to("/dir/user/signin");
				return;
			}

			std::string full_path(cfg->doc_root + "/dir_board_show.html");
			router::get_file(res, full_path);
			router::put_jsonp(req, res);
		}

		void show_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{
			jsonp_status_type status_code = status_error;
			boost::property_tree::ptree json, status, session, param, params;

			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				status_code = status_unauthorized;

				status.put("code", status_code);
				status.put("message", jsonp_status::str(status_code));

				json.put_child("status", status);

				std::stringstream ss;
				write_json(ss, json, false);

				res.content.append(jsonp_start);
				res.content.append(ss.str());
				res.content.append(jsonp_end);
				res.content_extension = "js";

				return;
			}

			status_code = status_okay;

			status.put("code", status_code);
			status.put("message", jsonp_status::str(status_code));

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

		void form_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				res.redirect_to("/dir/user/signin");
				return;
			}

			std::string full_path(cfg->doc_root + "/dir_board_form.html");
			router::get_file(res, full_path);
			router::put_jsonp(req, res);
		}

		void form_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{
			jsonp_status_type status_code = status_error;
			boost::property_tree::ptree json, status, session, param, params;

			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				status_code = status_unauthorized;

				status.put("code", status_code);
				status.put("message", jsonp_status::str(status_code));

				json.put_child("status", status);

				std::stringstream ss;
				write_json(ss, json, false);

				res.content.append(jsonp_start);
				res.content.append(ss.str());
				res.content.append(jsonp_end);
				res.content_extension = "js";

				return;
			}

			status_code = status_okay;

			status.put("code", status_code);
			status.put("message", jsonp_status::str(status_code));

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

		void remove(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

		void post(logger* log, config* cfg, database* db, request& req, response& res)
		{
			waspp::session sess(cfg, &req, &res);
			if (sess.get("userid").empty())
			{
				res.redirect_to("/dir/user/signin");
				return;
			}

			unsigned int userid = boost::lexical_cast<unsigned int>(sess.get("userid"));
			std::string& username = sess.get("username");
			
			unsigned int seq(0);
			std::string* title(0);
			std::string* content(0);
			
			// param check
			{
				if (!req.param("seq").empty())
				{
					seq = boost::lexical_cast<unsigned int>(req.param("seq"));
				}

				if (req.param("title").empty())
				{
					router::err_msg(res, "title is required", false);
					return;
				}
				*title = req.param("title");

				if (req.param("content").empty())
				{
					router::err_msg(res, "content is required", false);
					return;
				}
				*content = req.param("content");
			}

			dbconn_ptr db_etc = db->get("db_etc");
			{
				if (seq == 0)
				{
					stmt_ptr stmt(db_etc->prepare("CALL USP_GET_UNIQUE_KEYS('board', ?)"));
					{
						stmt->param(1);
					}

					res_ptr r(stmt->query());
					if (r->fetch(true))
					{
						seq = r->get<unsigned int>("last_key");
					}

					stmt.reset(db_etc->prepare("INSERT INTO board(seq, title, content, file1, file2, userid, username, inserttime, updatetime) VALUES(?, ?, ?, ?, ?, ?, ?, NOW(), NOW())"));
					{
						stmt->param(seq);
						stmt->param(*title);
						stmt->param(*content);
						stmt->param("");
						stmt->param("");
						stmt->param(userid);
						stmt->param(username);
					}

					unsigned long long int affected_rows = stmt->execute();
					if (affected_rows == 0)
					{
						router::err_msg(res, "insert_etc failed", db, "db_etc", db_etc);
						return;
					}
				}
				else
				{
					stmt_ptr stmt(db_etc->prepare("UPDATE board SET title = ?, content = ?, file1 = ?, file2 = ?, username = ?, updatetime = NOW() WHERE seq = ? AND userid = ?"));
					{
						stmt->param(*title);
						stmt->param(*content);
						stmt->param("");
						stmt->param("");
						stmt->param(username);
						stmt->param(seq);
						stmt->param(userid);
					}
					stmt->execute();
				}
			}
			db->free("db_etc", db_etc);

			res.redirect_to("/dir/board/index");
		}

	} // namespace dir_board

} // namespace waspp
