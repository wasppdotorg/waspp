/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/lexical_cast.hpp>

#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "request.hpp"
#include "response.hpp"
#include "mime_types.hpp"

namespace waspp
{
	namespace dir_user
	{
		namespace index
		{

			void html(logger* log, config* cfg, database* db, request& req, response& res)
			{
				dbconn_ptr db_index = db->get("db_index");
			
				unsigned int userid = 1;
				dbconn_ptr db_shard = db->get_shard(userid);
				

				res.status = response::ok;
				res.content_extension = "html";
				res.content = "OK";

				db->free("db_index", db_index);
				db->free_shard(userid, db_shard);
			}

			void jsonp(logger* log, config* cfg, database* db, request& req, response& res)
			{

			}

		} // namespace index

	} // namespace dir_user

} // namespace waspp
