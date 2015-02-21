//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "router.hpp"

namespace waspp
{
	namespace router
	{

		route routes[] =
		{
			//{ "/api/test", &api::test::json },

			{ "/board/index/", &board::index::html },
			{ "/?/board/index/", &board::index::jsonp },

			/*
				{ "/board/form", &board::form::html },
				{ "/?/board/form", &board::form::jsonp },
				{ "/board/post", &board::form::html },

				{ "/user/signin", &user::signin::html },
				{ "/?/user/signin", &user::signin::jsonp },
				{ "/user/auth", &user::auth },

				{ "/user/signup", &user::signup::html },
				{ "/?/user/signup", &user::signup::jsonp },
				{ "/user/post", &user::post },

				{ "/?/index", &index::jsonp },
				*/

			{ 0, 0 } // Marks end of list.
		};

		function_ptr find_func(const std::string& request_uri)
		{
			for (route* r = routes; r->uri; ++r)
			{
				if (!request_uri.find(r->uri))
				{
					return r->func;
				}
			}

			return 0;
		}

	} // namespace router
} // namespace waspp
