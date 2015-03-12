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

		}

	} // namespace dir_board

} // namespace waspp
