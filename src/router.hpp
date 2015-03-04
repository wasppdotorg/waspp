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

namespace waspp
{

	namespace dir_board
	{
		void index_html(logger* log, config* cfg, database* db, request& req, response& res);
		void index_jsonp(logger* log, config* cfg, database* db, request& req, response& res);

		void show_html(logger* log, config* cfg, database* db, request& req, response& res);
		void show_jsonp(logger* log, config* cfg, database* db, request& req, response& res);

		void form_html(logger* log, config* cfg, database* db, request& req, response& res);
		void form_jsonp(logger* log, config* cfg, database* db, request& req, response& res);

		void remove(logger* log, config* cfg, database* db, request& req, response& res);
		void post(logger* log, config* cfg, database* db, request& req, response& res);

	} // namespace board

	namespace dir_user
	{
		void signin_html(logger* log, config* cfg, database* db, request& req, response& res);
		void signup_html(logger* log, config* cfg, database* db, request& req, response& res);

		void auth(logger* log, config* cfg, database* db, request& req, response& res);
		void post(logger* log, config* cfg, database* db, request& req, response& res);

	} // namespace dir_user

	typedef void(*func_ptr)(logger*, config*, database* db, request&, response&);

	namespace router
	{

		struct route
		{
			const char* uri;
			func_ptr func;
		};

		func_ptr get_func(const std::string& request_uri);
		void res_file(response& res, const std::string& full_path);

	} // namespace router

} // namespace waspp

#endif // WASPP_ROUTER_HPP
