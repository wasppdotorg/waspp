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

namespace waspp
{
	namespace dir_board
	{
		struct msgpack_test
		{
			double b;
			std::string c;
		};

		void index_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			waspp::session sess(cfg, &req, &res);
			if (sess.get_sess("userid").empty())
			{
				res.redirect_to("/dir/user/signin");
				return;
			}

			std::string full_path(cfg->doc_root + "/dir_board_index.html");
			router::res_file(req, res, full_path);
		}

		void index_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{
			boost::property_tree::ptree json, status, session, param, params;

			waspp::session sess(cfg, &req, &res);
			if (sess.get_sess("userid").empty())
			{
				status.put("code", 4000);
				status.put("message", "");

				json.put_child("status", status);

				std::stringstream ss;
				write_json(ss, json, false);

				res.content = ss.str();
				res.content_extension = "json";

				return;
			}

			status.put("code", 2000);
			status.put("message", "OKAY");

			session.put("userid", "steve");
			session.put("username", "steve yune");

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

			res.content = ss.str();
			res.content_extension = "json";
		}

		void show_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			std::string full_path(cfg->doc_root + "/dir_board_show.html");
			router::res_file(req, res, full_path);
		}

		void show_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

		void form_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			std::string full_path(cfg->doc_root + "/dir_board_form.html");
			router::res_file(req, res, full_path);
		}

		void form_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

		void remove(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

		void post(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

	} // namespace dir_board

} // namespace waspp
