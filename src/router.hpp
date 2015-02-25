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

#include "response.hpp"
#include "request.hpp"

namespace waspp
{

	namespace board
	{
		namespace index
		{
			void html(request& req, response& res);
			void jsonp(request& req, response& res);
		} // namespace index
	} // namespace board

	typedef void(*func_ptr)(request&, response&);

	namespace router
	{

		struct route
		{
			const char* uri;
			func_ptr func;
		};

		func_ptr find_func(const std::string& request_uri);

	} // namespace router

} // namespace waspp

#endif // WASPP_ROUTER_HPP
