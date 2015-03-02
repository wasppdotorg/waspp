//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <map>
#include <string>

#include <fstream>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>

#include "request_handler.hpp"
#include "mime_types.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "database.hpp"
#include "response.hpp"
#include "request.hpp"
#include "router.hpp"

namespace waspp
{

	request_handler::request_handler(const std::string& doc_root_)
		: doc_root(doc_root_)
	{
	}

	void request_handler::handle_request(request& req, response& res)
	{
		logger* log = logger::instance();
		config* cfg = config::instance();
		database* db = database::instance();

		try
		{
			// Decode url to path.
			std::string request_uri;
			if (!url_validate_decode(req.uri, request_uri))
			{
				res = response::static_response(response::bad_request);
				return;
			}
			req.uri = request_uri;

			// Request path must be absolute and not contain "..".
			if (request_uri.empty() || request_uri[0] != '/'
				|| request_uri.find("..") != std::string::npos)
			{
				res = response::static_response(response::bad_request);
				return;
			}

			// If path ends in slash (i.e. is a directory) then add "index.html".
			if (request_uri[request_uri.size() - 1] == '/')
			{
				request_uri += "index.html";
			}

			// Determine the file extension.
			std::size_t last_slash_pos = request_uri.find_last_of("/");
			std::size_t last_dot_pos = request_uri.find_last_of(".");
			if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
			{
				res.content_extension = request_uri.substr(last_dot_pos + 1);
			}

			// Open the file to send back.
			std::string full_path = doc_root + request_uri;
			std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
			if (is)
			{
				// Fill out the response to be sent to the client.
				res.status = response::ok;
				char buf[512];
				while (is.read(buf, sizeof(buf)).gcount() > 0)
				{
					res.content.append(buf, is.gcount());
				}

				res.headers.resize(2);
				res.headers[0].name = "Content-Length";
				res.headers[0].value = boost::lexical_cast<std::string>(res.content.size());
				res.headers[1].name = "Content-Type";
				res.headers[1].value = mime_types::extension_to_type(res.content_extension);

				return;
			}

			// to make router::get_func to work correctly
			if (request_uri[request_uri.size() - 1] != '/')
			{
				request_uri += "/";
			}

			req.parse_cookie();
			req.parse_content();

			func_ptr func = router::get_func(request_uri);
			if (func == 0)
			{
				res = response::static_response(response::not_found);
				return;
			}

			func(log, cfg, db, req, res);

			res.headers.resize(2);
			res.headers[0].name = "Content-Length";
			res.headers[0].value = boost::lexical_cast<std::string>(res.content.size());
			res.headers[1].name = "Content-Type";
			res.headers[1].value = mime_types::extension_to_type(res.content_extension);

			std::string cookie;
			std::map<std::string, std::string>::iterator i;
			for (i = res.cookies.begin(); i != res.cookies.end(); ++i)
			{
				cookie.clear();
				cookie.append(i->first);
				cookie.append("=");
				cookie.append(i->second);
				cookie.append("; path=/");

				res.headers.push_back(name_value("Set-Cookie", cookie));
			}
		}
		catch (std::exception& e)
		{
			log->fatal(e.what());
			res = response::static_response(response::internal_server_error);
		}

		//res = response::static_response(response::not_found);
		//return;

		/*
			std::vector<std::string> request_params;
			boost::split(request_params, request_path, boost::is_any_of("/"));
			request_params.erase(request_params.begin());

			boost::property_tree::ptree pt, response, session, params, headers_;

			response.put("status", 2000);
			response.put("message", "OKAY");
			response.put("method", req.method);
			response.put("uri", req.uri);
			response.put("version1", req.http_version_major);
			response.put("version2", req.http_version_minor);

			std::vector<header> headers;

			session.put("userid", "steve");
			session.put("username", "steve yune");

			std::vector<std::string>::iterator i;
			for (i = request_params.begin(); i != request_params.end(); ++i)
			{
			params.push_back(std::make_pair("", *i));
			}

			pt.put_child("response", response);
			pt.put_child("session", session);
			pt.put_child("params", params);
			pt.put_child("headers_", headers_);

			std::stringstream ss;
			write_json(ss, pt, false);

			// Fill out the response to be sent to the client.
			res.status = response::ok;
			res.content = ss.str();

			res.headers.push_back(name_value("Content-Length", boost::lexical_cast<std::string>(res.content.size())));
			res.headers.push_back(name_value("Content-Type", mime_types::extension_to_type("json")));

			*/
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
