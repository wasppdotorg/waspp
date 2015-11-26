//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_REQUEST_HPP
#define WASPP_REQUEST_HPP

#include <string>
#include <vector>
#include <algorithm>

#include <boost/unordered_map.hpp>
#include <boost/bind.hpp>

#include "name_value.hpp"
#include "multipart.hpp"

namespace waspp
{

	/// A request received from a client.
	struct request
	{
		std::string& header(const std::string& name)
		{
			std::vector<name_value>::iterator found;
			found = std::find_if(headers.begin(), headers.end(), boost::bind(&name_value::compare_name, _1, name));

			if (found == headers.end())
			{
				headers.push_back(name_value(name, std::string()));
				found = headers.end() - 1;
			}

			return found->value;
		}

		std::string& cookie(const std::string& name)
		{
			std::vector<name_value>::iterator found;
			found = std::find_if(cookies.begin(), cookies.end(), boost::bind(&name_value::compare_name, _1, name));

			if (found == cookies.end())
			{
				cookies.push_back(name_value(name, std::string()));
				found = cookies.end() - 1;
			}

			return found->value;
		}

		std::string& param(const std::string& name)
		{
			std::vector<name_value>::iterator found;
			found = std::find_if(params.begin(), params.end(), boost::bind(&name_value::compare_name, _1, name));

			if (found == params.end())
			{
				params.push_back(name_value(name, std::string()));
				found = params.end() - 1;
			}

			return found->value;
		}

		void parse_connection_header()
		{
			conn_header = 'K';
			if (http_version_major == 1 && http_version_minor == 0)
			{
				conn_header = 'C';
			}

			if (!header("Conection").empty())
			{
				conn_header = std::toupper(header("Connection")[0]);
			}
		}

		void reset()
		{
			remote_endpoint.clear();
			conn_header = 'K';

			method.clear();
			uri.clear();
			http_version_major = 0;
			http_version_minor = 0;
			headers.clear();

			content.clear();
			content_length = 0;

			cookies.clear();
			params.clear();
			rest_params.clear();
			uploads.clear();
		}

		std::string remote_endpoint;
		char conn_header;

		std::string method;
		std::string uri;
		int http_version_major;
		int http_version_minor;
		std::vector<name_value> headers;

		std::string content;
		int content_length;

		std::vector<name_value> cookies;
		std::vector<name_value> params;
		std::vector<std::string> rest_params;
		std::vector<multipart_content> uploads;

	};

} // namespace waspp

#endif // WASPP_REQUEST_HPP
