/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef router_hpp
#define router_hpp

#include "config.hpp"
#include "request.hpp"
#include "response.hpp"

namespace waspp
{

	namespace dir_forum
	{
		
		void index_html(config* cfg, request& req, response& res);
		void index_jsonp(config* cfg, request& req, response& res);

		void show_html(config* cfg, request& req, response& res);
		void show_jsonp(config* cfg, request& req, response& res);

		void form_html(config* cfg, request& req, response& res);
		void form_jsonp(config* cfg, request& req, response& res);

		void del(config* cfg, request& req, response& res);
		void post(config* cfg, request& req, response& res);

	} // namespace dir_forum

	namespace dir_user
	{
		
		void signin_html(config* cfg, request& req, response& res);
		void auth(config* cfg, request& req, response& res);

		void signup_html(config* cfg, request& req, response& res);
		void post(config* cfg, request& req, response& res);

		void signout(config* cfg, request& req, response& res);

	} // namespace dir_user

	namespace dir_performance
	{
		
		void test(config* cfg, request& req, response& res);

	} // namespace dir_performance

	using func_ptr = void(*)(config*, request&, response&);

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
		bool get_file(config* cfg, response& res, const std::string& request_path);

		void set_jsonp(request& req, response& res);

		void err_msg(config* cfg, response& res, error_type err_code);

	} // namespace router

} // namespace waspp

#endif // router_hpp
