/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/lexical_cast.hpp>

#include "mime_types.hpp"
#include "router.hpp"

namespace waspp
{
	namespace router
	{

		route routes[] =
		{
			{ "/dir/board/index/", &dir_board::index::html },
			{ "/_/dir/board/index/", &dir_board::index::jsonp },

			/*
			{ "/dir/board/show/", &dir_board::show::html },
			{ "/_/dir/board/show/", &dir_board::show::jsonp },

			{ "/dir/board/form/", &dir_board::form::html },
			{ "/_/dir/board/form/", &dir_board::form::jsonp },

			{ "/dir/board/remove/", &dir_board::remove::html },
			{ "/_/dir/board/remove/", &dir_board::remove::jsonp },

			{ "/dir/board/post/", &dir_board::post::html },
			{ "/_/dir/board/post/", &dir_board::post::jsonp },

			{ "/dir/user/signin/", &dir_user::signin::html },
			{ "/_/dir/user/signin/", &dir_user::signin::jsonp },

			{ "/dir/user/signup/", &dir_user::signup::html },
			{ "/_/dir/user/signup/", &dir_user::signup::jsonp },
			*/

			{ 0, 0 } // Marks end of list.
		};

		func_ptr get_func(const std::string& request_uri)
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

		void res_file(response& res, const std::string& full_path)
		{
			std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
			if (!is)
			{
				return;
			}

			// Fill out the response to be sent to the client.
			res.status = response::ok;
			char buf[512];
			while (is.read(buf, sizeof(buf)).gcount() > 0)
			{
				res.content.append(buf, is.gcount());
			}

			res.headers.resize(2);
			res.headers[0].name = "Content-Length";
			res.headers[0].value = boost::lexical_cast<std::string>(res.content.size());
			res.headers[1].name = "Content-Type";
			res.headers[1].value = mime_types::extension_to_type(get_extension(full_path));

			res.finished = true;
		}

	} // namespace router

} // namespace waspp
