/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_ROUTER_HPP
#define WASPP_ROUTER_HPP

#include <string>

#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "request.hpp"
#include "response.hpp"
#include "error.hpp"

namespace waspp
{

	namespace dir_forum
	{
		void index_html(config* cfg, database* db, request& req, response& res);
		void index_jsonp(config* cfg, database* db, request& req, response& res);

		void show_html(config* cfg, database* db, request& req, response& res);
		void show_jsonp(config* cfg, database* db, request& req, response& res);

		void form_html(config* cfg, database* db, request& req, response& res);
		void form_jsonp(config* cfg, database* db, request& req, response& res);

		void remove(config* cfg, database* db, request& req, response& res);
		void post(config* cfg, database* db, request& req, response& res);

	} // namespace dir_forum

	namespace dir_user
	{
		void signin_html(config* cfg, database* db, request& req, response& res);
		void auth(config* cfg, database* db, request& req, response& res);

		void signup_html(config* cfg, database* db, request& req, response& res);
		void post(config* cfg, database* db, request& req, response& res);

		void signout(config* cfg, database* db, request& req, response& res);

	} // namespace dir_user

	typedef void(*func_ptr)(config*, database*, request&, response&);

	const std::string jsonp_start = "_(";
	const std::string jsonp_end = ");";

	namespace router
	{

		struct route
		{
			const char* uri;
			func_ptr func;
		};

		func_ptr get_func(std::string& request_uri);
		bool get_file(config* cfg, response& res, std::string full_path);

		void set_jsonp(request& req, response& res);

		void err_msg(config* cfg, response& res, error_type err_code);

	} // namespace router

} // namespace waspp

#endif // WASPP_ROUTER_HPP
