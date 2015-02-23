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
			void html(const request& req, response& res);
			void jsonp(const request& req, response& res);
		} // namespace index
	} // namespace board

	typedef void(*function_ptr)(const request&, response&);

	namespace router
	{

		struct route
		{
			const char* uri;
			function_ptr func;
		};

		function_ptr find_func(const std::string& request_uri);

	} // namespace router

} // namespace waspp

#endif // WASPP_ROUTER_HPP