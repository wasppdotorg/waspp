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
#include "app_status.hpp"

namespace waspp
{

	namespace dir_forum
	{
		void index_html(logger* log, config* cfg, database* db, request& req, response& res);
		void index_jsonp(logger* log, config* cfg, database* db, request& req, response& res);

		void show_html(logger* log, config* cfg, database* db, request& req, response& res);
		void show_jsonp(logger* log, config* cfg, database* db, request& req, response& res);

		void form_html(logger* log, config* cfg, database* db, request& req, response& res);
		void form_jsonp(logger* log, config* cfg, database* db, request& req, response& res);

		void remove(logger* log, config* cfg, database* db, request& req, response& res);
		void post(logger* log, config* cfg, database* db, request& req, response& res);

	} // namespace dir_forum

	namespace dir_user
	{
		void signin_html(logger* log, config* cfg, database* db, request& req, response& res);
		void auth(logger* log, config* cfg, database* db, request& req, response& res);

		void signup_html(logger* log, config* cfg, database* db, request& req, response& res);
		void post(logger* log, config* cfg, database* db, request& req, response& res);

		void signout(logger* log, config* cfg, database* db, request& req, response& res);

	} // namespace dir_user

	typedef void(*func_ptr)(logger*, config*, database*, request&, response&);

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

		void put_jsonp(request& req, response& res);

		void err_msg(config* cfg, response& res, app_status_type status_code, bool has_db);
		void err_msg(config* cfg, response& res, app_status_type status_code, database* db, const std::string& dbname, dbconn_ptr dbconn);
		void err_msg_shard(config* cfg, response& res, app_status_type status_code, database* db, unsigned int shard_key, dbconn_ptr dbconn);

	} // namespace router

} // namespace waspp

#endif // WASPP_ROUTER_HPP
