//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_ROUTER_HPP
#define WASPP_ROUTER_HPP

#include <map>
#include <string>

#include "config.hpp"
#include "logger.hpp"
#include "database.hpp"
#include "request.hpp"
#include "response.hpp"

namespace waspp
{

	namespace board
	{
		namespace index
		{
			void html(config* cfg, logger* log, database* db, request& req, response& res);
			void jsonp(config* cfg, logger* log, database* db, request& req, response& res);
		} // namespace index
	} // namespace board

	typedef void(*func_ptr)(config*, logger*, database* db, request&, response&);

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
