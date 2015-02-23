//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "response.hpp"
#include "request.hpp"
#include "mime_types.hpp"
#include "database.hpp"

namespace waspp
{
	namespace board
	{
		namespace index
		{

			void html(const request& req, response& res)
			{
				database* db = database::instance();

				dbconn_ptr db_index = db->get("db_index");
				db->free("db_index", db_index);

				dbconn_ptr db_shard = db->get(1);
				db->free(1, db_shard);

				res.status = response::ok;
				res.content = "OK";
				res.headers.resize(2);
				res.headers[0].key = "Content-Length";
				res.headers[0].value = "2";
				res.headers[1].key = "Content-Type";
				res.headers[1].value = mime_types::extension_to_type("html");
			}

			void jsonp(const request& req, response& res)
			{

			}

		} // namespace index
	} // namespace board
} // namespace waspp