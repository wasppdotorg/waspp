//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "request.hpp"

namespace waspp
{

	void request::parse_connection_header()
	{
		connection_option = 'K'; // Connection: Keep-Alive
		if (http_version_major_ == 1 && http_version_minor_ == 0)
		{
			connection_option = 'c'; // Connection: close
		}

		if (!header("Connection").empty())
		{
			connection_option = header("Connection")[0];
		}
	}

	std::string& request::header(const std::string& name)
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

	std::string& request::cookie(const std::string& name)
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

	std::string& request::param(const std::string& name)
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

} // namespace waspp

