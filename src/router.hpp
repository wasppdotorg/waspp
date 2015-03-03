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

	namespace board
	{

		namespace index
		{
			void html(logger* log, config* cfg, database* db, request& req, response& res);
			void jsonp(logger* log, config* cfg, database* db, request& req, response& res);

		} // namespace index

	} // namespace board

	typedef void(*func_ptr)(logger*, config*, database* db, request&, response&);

	namespace router
	{

		struct route
		{
			const char* uri;
			func_ptr func;
		};

		func_ptr get_func(const std::string& request_uri);

	} // namespace router

} // namespace waspp

#endif // WASPP_ROUTER_HPP
