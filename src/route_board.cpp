//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "response.hpp"
#include "request.hpp"
#include "cookie.hpp"
#include "database.hpp"
#include "mime_types.hpp"

namespace waspp
{
	namespace board
	{
		namespace index
		{

			void html(request& req, response& res)
			{
				cookie cookie_(req);
				database* db = database::instance();

				dbconn_ptr db_index = db->get("db_index");
				db->free("db_index", db_index);

				unsigned int userid = 1;
				dbconn_ptr db_shard = db->get_shard(userid);
				db->free_shard(userid, db_shard);

				res.status = response::ok;
				res.content = "OK";

				res.headers.resize(0);
				res.headers.push_back(key_value("Content-Length", "2"));
				res.headers.push_back(key_value("Content-Type", mime_types::extension_to_type("html")));

				std::map<std::string, std::string>::iterator i;
				for (i = cookie_.all().begin(); i != cookie_.all().end(); ++i)
				{
					res.headers.push_back(key_value("Set-Cookie", cookie_.str(i)));
				}
			}

			void jsonp(request& req, response& res)
			{

			}

		} // namespace index
	} // namespace board
} // namespace waspp
