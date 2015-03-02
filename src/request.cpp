//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>

#include "request.hpp"

namespace waspp
{

	std::string request::get_header(const std::string& name)
	{
		std::vector<name_value>::iterator found;
		found = std::find_if(headers.begin(), headers.end(), boost::bind(&name_value::compare_name, _1, name));

		if (found == headers.end())
		{
			return std::string();
		}

		return found->value;
	}

	std::string& request::get_cookie(const std::string& name)
	{
		return cookies[name];
	}

	std::string request::get_param(const std::string& name)
	{
		std::vector<name_value>::iterator found;
		found = std::find_if(params.begin(), params.end(), boost::bind(&name_value::compare_name, _1, name));

		if (found == params.end())
		{
			return std::string();
		}

		return found->value;
	}

	void request::parse_cookie()
	{
		std::string cookie_header = get_header("Cookie");
		if (cookie_header.empty())
		{
			return;
		}

		std::vector<std::string> cookies_splited;
		boost::split(cookies_splited, cookie_header, boost::is_any_of(";"));

		std::size_t last_pos;
		for (std::size_t i = 0; i < cookies_splited.size(); ++i)
		{
			boost::algorithm::trim(cookies_splited[i]);

			last_pos = cookies_splited[i].find_last_of("=");
			if (last_pos == std::string::npos)
			{
				continue;
			}

			cookies[cookies_splited[i].substr(0, last_pos)] = cookies_splited[i].substr(last_pos + 1);
		}
	}

	void request::parse_content()
	{
		if (method == "POST")
		{
		}
	}

} // namespace waspp
