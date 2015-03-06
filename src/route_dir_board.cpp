/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/algorithm/string.hpp>
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

namespace waspp
{
	namespace dir_board
	{

		void index_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			std::string full_path(cfg->doc_root + "/dir_board_index.html");
			router::res_file(res, full_path);
		}

		void index_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{
			std::vector<std::string> request_params;
			boost::split(request_params, req.uri, boost::is_any_of("/"));
			request_params.erase(request_params.begin());

			boost::property_tree::ptree pt, response, session, params, headers_;

			response.put("status", 2000);
			response.put("message", "OKAY");
			response.put("method", req.method);
			response.put("uri", req.uri);
			response.put("version1", req.http_version_major);
			response.put("version2", req.http_version_minor);

			std::vector<name_value> headers;

			session.put("userid", "steve");
			session.put("username", "steve yune");

			std::vector<std::string>::iterator i;
			for (i = request_params.begin(); i != request_params.end(); ++i)
			{
				//params.push_back(name_value("", *i));
				//params.push_back(name_value(std::string(), *i));
			}

			pt.put_child("response", response);
			pt.put_child("session", session);
			pt.put_child("params", params);
			pt.put_child("headers_", headers_);

			std::stringstream ss;
			write_json(ss, pt, false);

			res.content = ss.str();
			res.content_extension = "json";
		}

		void show_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			std::string full_path(cfg->doc_root + "/dir_board_show.html");
			router::res_file(res, full_path);
		}

		void show_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

		void form_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			std::string full_path(cfg->doc_root + "/dir_board_form.html");
			router::res_file(res, full_path);
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
