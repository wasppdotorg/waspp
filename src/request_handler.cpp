//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request_handler.hpp"
#include "logger.hpp"
#include "router.hpp"
#include "mime_types.hpp"
#include "utility.hpp"

namespace waspp
{

	request_handler::request_handler() : cfg(*config::instance())
	{

	}

	void request_handler::handle_request(request& req, response& res)
	{
		std::string request_uri;

		try
		{
			if (!is_access_granted(req.remote_addr) || is_access_denied(req.remote_addr))
			{
				res = response::static_response(response::unauthorized);
				log(error) << response::unauthorized << "," << request_uri << "," << req.remote_addr;
				return;
			}

			if (!percent_decode_and_validate(req.uri, request_uri))
			{
				res = response::static_response(response::bad_request);
				log(error) << response::bad_request << "," << request_uri << "," << req.remote_addr;
				return;
			}

			// Request path must be absolute and not contain "..".
			if (request_uri.empty() || request_uri[0] != '/'
				|| request_uri.find("..") != std::string::npos)
			{
				res = response::static_response(response::bad_request);
				log(error) << response::bad_request << "," << request_uri << "," << req.remote_addr;
				return;
			}
			std::string request_path(request_uri);

			auto func = router::get_func(request_uri);
			if (func == nullptr)
			{
				if (!router::get_file(cfg, res, request_path))
				{
					res = response::static_response(response::not_found);
					log(error) << response::not_found << "," << request_uri << "," << req.remote_addr;
					return;
				}
			}
			else
			{
				func(cfg, req, res);
			}

			// Fill out the response to be sent to the client.
			res.headers.push_back(name_value("Content-Type", mime_types::extension_to_type(res.content_extension)));
			res.headers.push_back(name_value("Keep-Alive", "timeout=0, max=0"));

			std::string cookie;
			for (auto& res_cookie : res.cookies)
			{
				cookie.clear();
				cookie.append(res_cookie.name);
				cookie.append("=");
				cookie.append(res_cookie.value);
				cookie.append("; path=/");

				res.headers.push_back(name_value("Set-Cookie", cookie));
			}

			if (cfg.compress() &&
				req.header("Accept-Encoding") == "gzip, deflate" &&
				mime_types::is_compressible(res.content_extension))
			{
				res.headers.push_back(name_value("Content-Encoding", "gzip"));
				gzip_str(res.content);
			}

			res.headers.push_back(name_value("Content-Length", std::to_string(res.content.size())));
			res.status = response::ok;

			log(info) << response::ok << "," << request_uri << "," << req.remote_addr;
		}
		catch (std::exception& e)
		{
			res = response::static_response(response::internal_server_error);
			log(fatal) << response::internal_server_error << "," << request_uri << "," << req.remote_addr << "," << e.what() << "," << __FILE__ << ":" << __LINE__;
		}
	}

	bool request_handler::is_access_granted(const std::string& remote_addr)
	{
		if (cfg.access_granted().size() == 0)
		{
			return true;
		}

		if (std::find(cfg.access_granted().begin(), cfg.access_granted().end(), remote_addr) != cfg.access_granted().end())
		{
			return false;
		}

		return true;
	}

	bool request_handler::is_access_denied(const std::string& remote_addr)
	{
		if (cfg.access_denied().size() == 0)
		{
			return false;
		}

		if (std::find(cfg.access_denied().begin(), cfg.access_denied().end(), remote_addr) == cfg.access_denied().end())
		{
			return true;
		}

		return false;
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
