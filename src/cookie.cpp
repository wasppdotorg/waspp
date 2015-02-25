//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>
#include <algorithm>
#include <string>
#include <sstream>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include "request.hpp"
#include "response.hpp"
#include "key_value.hpp"
#include "cookie.hpp"

namespace waspp
{

	cookie::cookie(request& req)
	{
		std::vector<key_value>::const_iterator found;
		found = std::find_if(req.headers.begin(), req.headers.end(), boost::bind(&key_value::compare_key, _1, "Cookie"));

		if (found == req.headers.end())
		{
			return;
		}

		std::vector<std::string> cookies;
		boost::split(cookies, found->value, boost::is_any_of(";"));

		std::size_t last_pos;
		std::size_t lws_count;
		for (std::size_t i = 0; i < cookies.size(); ++i)
		{
			last_pos = cookies[i].find_last_of("=");
			if (last_pos == std::string::npos)
			{
				continue;
			}

			lws_count = 0;
			for (std::size_t j = 0; j < cookies[i].size(); ++j, ++lws_count)
			{
				if (std::isspace(cookies[i][j]) == 0)
				{
					break;
				}
			}

			cookie_.insert(std::make_pair(cookies[i].substr(lws_count, last_pos - lws_count), cookies[i].substr(++last_pos)));
		}
	}

	cookie::~cookie()
	{
	}

	std::string& cookie::get_cookie(const std::string& name)
	{
		return cookie_[name];
	}

	void cookie::set_cookie(const std::string& name, const std::string& value)
	{
		cookie_[name] = value;
	}

	void cookie::delete_cookie(const std::string& name)
	{
		set_cookie(name, std::string());
	}

	std::map<std::string, std::string>::iterator cookie::begin()
	{
		return cookie_.begin();
	}

	std::map<std::string, std::string>::iterator cookie::end()
	{
		return cookie_.end();
	}

	std::string cookie::str(std::map<std::string, std::string>::iterator i)
	{
		std::string value;
		{
			value.append(i->first);
			value.append("=");
			value.append(i->second);
			value.append("; path=/");
		}

		return value;
	}

} // namespace waspp
