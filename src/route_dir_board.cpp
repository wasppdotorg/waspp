/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/lexical_cast.hpp>

#include "request_handler.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "request.hpp"
#include "response.hpp"
#include "mime_types.hpp"
#include "router.hpp"

namespace waspp
{
	namespace dir_board
	{
		namespace index
		{

			void html(logger* log, config* cfg, database* db, request& req, response& res)
			{
				router::res_file(res, cfg->doc_root + "/dir_board_index.html");
			}

			void jsonp(logger* log, config* cfg, database* db, request& req, response& res)
			{

			}

		} // namespace index

	} // namespace dir_board

} // namespace waspp
