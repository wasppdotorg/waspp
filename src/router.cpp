/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/lexical_cast.hpp>

#include "router.hpp"
#include "config.hpp"
#include "database.hpp"

namespace waspp
{
	namespace router
	{

		route routes[] =
		{
			{ "/dir/forum/index/", &dir_forum::index_html },
			{ "/_/dir/forum/index/", &dir_forum::index_jsonp },

			{ "/dir/forum/show/", &dir_forum::show_html },
			{ "/_/dir/forum/show/", &dir_forum::show_jsonp },

			{ "/dir/forum/form/", &dir_forum::form_html },
			{ "/_/dir/forum/form/", &dir_forum::form_jsonp },

			{ "/dir/forum/remove/", &dir_forum::remove },
			{ "/dir/forum/post/", &dir_forum::post },

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

		bool get_file(config* cfg, response& res, std::string request_path)
		{
			std::string full_path(cfg->doc_root + request_path);

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

		void put_jsonp(request& req, response& res)
		{
			res.content.append("\n<script src='/_.js'></script>\n<script src='/_");
			res.content.append(req.uri);
			res.content.append("'></script>\n");
		}

		void err_msg(config* cfg, response& res, app_status_type status_code, bool has_db)
		{
			if (has_db)
			{
				throw std::runtime_error("db must be free");
			}

			res.content = cfg->msg(status_code);
			res.content_extension = "html";
		}

		void err_msg(config* cfg, response& res, app_status_type status_code, database* db, const std::string& dbname, dbconn_ptr dbconn)
		{
			err_msg(cfg, res, status_code, false);
			db->free(dbname, dbconn);
		}

		void err_msg_shard(config* cfg, response& res, app_status_type status_code, database* db, unsigned int shard_key, dbconn_ptr dbconn)
		{
			err_msg(cfg, res, status_code, false);
			db->free_shard(shard_key, dbconn);
		}

	} // namespace router

} // namespace waspp
