//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>

#include <string>
#include <sstream>

#include <boost/timer/timer.hpp>
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

	request_handler::request_handler() : cfg(config::instance()), db(database::instance())
	{

	}

	void request_handler::handle_request(request& req, response& res)
	{
		std::string request_uri;

		try
		{
			if (cfg->granted().size() > 0 || cfg->denied().size() > 0)
			{
				std::size_t pos = req.remote_endpoint.find(":");
				if (pos == std::string::npos)
				{
					res = response::static_response(response::bad_request);
					log(error) << response::bad_request << "," << request_uri << "," << req.remote_endpoint;
					return;
				}

				std::string remote_addr = req.remote_endpoint.substr(0, pos);
				for (std::size_t i = 0; i < cfg->granted().size(); ++i)
				{
					if (remote_addr != cfg->granted()[i])
					{
						res = response::static_response(response::unauthorized);
						log(error) << response::unauthorized << "," << request_uri << "," << req.remote_endpoint;
						return;
					}
				}

				for (std::size_t i = 0; i < cfg->denied().size(); ++i)
				{
					if (remote_addr == cfg->granted()[i])
					{
						res = response::static_response(response::unauthorized);
						log(error) << response::unauthorized << "," << request_uri << "," << req.remote_endpoint;
						return;
					}
				}
			}
		
			if (!percent_decode_and_validate(req.uri, request_uri))
			{
				res = response::static_response(response::bad_request);
				log(error) << response::bad_request << "," << request_uri << "," << req.remote_endpoint;
				return;
			}

			// Request path must be absolute and not contain "..".
			if (request_uri.empty() || request_uri[0] != '/'
				|| request_uri.find("..") != std::string::npos)
			{
				res = response::static_response(response::bad_request);
				log(error) << response::bad_request << "," << request_uri << "," << req.remote_endpoint;
				return;
			}
			std::string request_path(request_uri);

			func_ptr func = router::get_func(request_uri);
			if (func == 0)
			{
				if (!router::get_file(cfg, res, request_path))
				{
					res = response::static_response(response::not_found);
					log(error) << response::not_found << "," << request_path << "," << req.remote_endpoint;
					return;
				}
			}
			else
			{
				func(cfg, db, req, res);
			}

			// Fill out the response to be sent to the client.
			res.status = response::ok;

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
			log(info) << response::ok << "," << request_path << "," << req.remote_endpoint;
		}
		catch (std::exception& e)
		{
			res = response::static_response(response::internal_server_error);
			log(fatal) << response::internal_server_error << "," << request_uri << "," << req.remote_endpoint << "," << e.what() << "," << __FILE__ << ":" << __LINE__;
		}
	}

	bool request_handler::percent_decode_and_validate(const std::string& in, std::string& out)
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
