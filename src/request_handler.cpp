//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>
#include <sstream>

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
			std::string request_uri;
			if (!url_validate_decode(req.uri, request_uri))
			{
				res = response::static_response(response::bad_request);
				return;
			}

			// Request path must be absolute and not contain "..".
			if (request_uri.empty() || request_uri[0] != '/'
				|| request_uri.find("..") != std::string::npos)
			{
				res = response::static_response(response::bad_request);
				return;
			}
			std::string request_path(request_uri);

			func_ptr func = router::get_func(request_uri);
			if (func == 0)
			{
				if (!router::get_file(cfg, res, request_path))
				{
					res = response::static_response(response::not_found);
					return;
				}
			}
			else
			{
				func(log, cfg, db, req, res);
			}

			// Fill out the response to be sent to the client.
			res.http_status = response::ok;

			res.headers.push_back(name_value("Content-Type", mime_types::extension_to_type(res.content_extension)));
			res.headers.push_back(name_value("Keep-Alive", "timeout=0, max=0"));

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

			if (cfg->compress() &&
				req.header("Accept-Encoding") == "gzip, deflate" &&
				mime_types::is_compressible(res.content_extension))
			{
				res.headers.push_back(name_value("Content-Encoding", "gzip"));
				gzip_str(res.content);
			}

			res.headers.push_back(name_value("Content-Length", boost::lexical_cast<std::string>(res.content.size())));
		}
		catch (std::exception& e)
		{
			log->fatal(__FILE__, __LINE__, e.what());
			res = response::static_response(response::internal_server_error);
		}
	}

	bool request_handler::url_validate_decode(const std::string& in, std::string& out)
	{
		out.clear();
		out.reserve(in.size());
		for (std::size_t i = 0; i < in.size(); ++i)
		{
			if (in[i] == '%')
			{
				if (i + 3 <= in.size())
				{
					int value = 0;
					std::istringstream is(in.substr(i + 1, 2));
					if (is >> std::hex >> value)
					{
						out += static_cast<char>(value);
						i += 2;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
			else if (in[i] == '+')
			{
				out += ' ';
			}
			else
			{
				out += in[i];
			}
		}
		return true;
	}

} // namespace waspp
