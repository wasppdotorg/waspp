/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/lexical_cast.hpp>

#include "router.hpp"
#include "mime_types.hpp"

namespace waspp
{
	namespace router
	{

		route routes[] =
		{
			{ "/dir/board/index/", &dir_board::index_html },
			{ "/_/dir/board/index/", &dir_board::index_jsonp },

			{ "/dir/board/show/", &dir_board::show_html },
			{ "/_/dir/board/show/", &dir_board::show_jsonp },

			{ "/dir/board/form/", &dir_board::form_html },
			{ "/_/dir/board/form/", &dir_board::form_jsonp },

			{ "/dir/board/remove/", &dir_board::remove },
			{ "/dir/board/post/", &dir_board::post },

			{ "/dir/user/signin/", &dir_user::signin_html },
			{ "/dir/user/auth/", &dir_user::auth },

			{ "/dir/user/signup/", &dir_user::signup_html },
			{ "/dir/user/post/", &dir_user::post },

			{ "/dir/user/signout/", &dir_user::signout },

			{ 0, 0 } // Marks end of list.
		};

		func_ptr get_func(std::string& request_uri)
		{
			// to make router::get_func to work correctly
			if (request_uri[request_uri.size() - 1] != '/')
			{
				request_uri += "/";
			}

			for (route* r = routes; r->uri; ++r)
			{
				if (!request_uri.find(r->uri))
				{
					return r->func;
				}
			}

			return 0;
		}

		void res_file(request& req, response& res, std::string& full_path)
		{
			std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
			if (!is)
			{
				res = response::static_response(response::not_found);
				return;
			}

			// Fill out the response to be sent to the client.
			res.status = response::ok;

			char buf[512];
			while (is.read(buf, sizeof(buf)).gcount() > 0)
			{
				res.content.append(buf, is.gcount());
			}

			std::string jsonp_request_uri("__WASPP_JSONP_REQUEST_URI__");
			std::size_t found = res.content.find(jsonp_request_uri);
			if (found != std::string::npos)
			{
				res.content.replace(found, jsonp_request_uri.size(), "/_" + req.uri);
			}
			
			res.headers.resize(3);
			res.headers[0].name = "Content-Length";
			res.headers[0].value = boost::lexical_cast<std::string>(res.content.size());
			res.headers[1].name = "Content-Type";
			res.headers[1].value = mime_types::extension_to_type(get_extension(full_path));
			res.headers[2].name = "Keep-Alive";
			res.headers[2].value = "timeout=0, max=0";

			res.finished = true;
		}

	} // namespace router

} // namespace waspp
