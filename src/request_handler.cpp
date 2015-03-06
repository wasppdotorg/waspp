//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

#include "request_handler.hpp"
#include "request.hpp"
#include "response.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "router.hpp"
#include "mime_types.hpp"

namespace waspp
{

	request_handler::request_handler()
	{
	}

	void request_handler::handle_request(request& req, response& res)
	{
		logger* log = logger::instance();
		config* cfg = config::instance();
		database* db = database::instance();

		try
		{
			// Request path must be absolute and not contain "..".
			if (req.uri.empty() || req.uri[0] != '/'
				|| req.uri.find("..") != std::string::npos)
			{
				res = response::static_response(response::bad_request);
				return;
			}

			std::string full_path = cfg->doc_root + req.uri;
			if (boost::filesystem::exists(full_path))
			{
				router::res_file(res, full_path);
				return;
			}

			func_ptr func = router::get_func(req.uri);
			if (func == 0)
			{
				res = response::static_response(response::not_found);
				return;
			}

			func(log, cfg, db, req, res);
			if (res.finished)
			{
				return;
			}

			res.status = response::ok;

			res.headers.resize(2);
			res.headers[0].name = "Content-Length";
			res.headers[0].value = boost::lexical_cast<std::string>(res.content.size());
			res.headers[1].name = "Content-Type";
			res.headers[1].value = mime_types::extension_to_type(res.content_extension);

			std::string cookie;
			for (std::size_t i = 0; i < res.cookies.size(); ++i)
			{
				cookie.clear();
				cookie.append(res.cookies[i].name);
				cookie.append("=");
				cookie.append(res.cookies[i].value);
				cookie.append("; path=/");

				res.headers.push_back(name_value("Set-Cookie", cookie));
			}
		}
		catch (std::exception& e)
		{
			log->fatal(e.what());
			res = response::static_response(response::internal_server_error);
		}
	}

} // namespace waspp
