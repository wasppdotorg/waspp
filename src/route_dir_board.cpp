/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include "router.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "request.hpp"
#include "response.hpp"

namespace waspp
{
	namespace dir_board
	{

		void index_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			router::res_file(res, cfg->doc_root + "/dir_board_index.html");
		}

		void index_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

		void show_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			router::res_file(res, cfg->doc_root + "/dir_board_show.html");
		}

		void show_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

		void form_html(logger* log, config* cfg, database* db, request& req, response& res)
		{
			router::res_file(res, cfg->doc_root + "/dir_board_form.html");
		}

		void form_jsonp(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

		void remove(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

		void post(logger* log, config* cfg, database* db, request& req, response& res)
		{

		}

	} // namespace dir_board

} // namespace waspp
