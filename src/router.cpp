/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/lexical_cast.hpp>

#include "router.hpp"
#include "database.hpp"

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

		bool get_file(response& res, std::string& full_path)
		{
			// If path ends in slash (i.e. is a directory) then add "index.html".
			if (full_path[full_path.size() - 1] == '/')
			{
				full_path += "index.html";
			}

			std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
			if (!is)
			{
				return false;
			}

			char buf[512];
			while (is.read(buf, sizeof(buf)).gcount() > 0)
			{
				res.content.append(buf, is.gcount());
			}
			res.content_extension = get_extension(full_path);

			return true;
		}

		void err_msg(response& res, const std::string& message, bool has_db)
		{
			if (!has_db)
			{
				res.content = message;
				res.content_extension = "html";
			}
		}

		void err_msg(response& res, const std::string& message, database* db, const std::string& dbname, dbconn_ptr dbconn)
		{
			err_msg(res, message, false);
			db->free(dbname, dbconn);
		}

	} // namespace router

} // namespace waspp
