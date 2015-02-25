//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>
#include <algorithm>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include "cookie.hpp"
#include "key_value.hpp"

namespace waspp
{

	cookie::cookie(std::vector<key_value>& headers)
	{
		std::vector<key_value>::iterator found;
		found = std::find_if(headers.begin(), headers.end(), boost::bind(&key_value::compare_key, _1, "Cookie"));

		if (found == headers.end())
		{
			return;
		}

		std::vector<std::string> cookie_params;
		boost::split(cookie_params, found->value, boost::is_any_of("; "));

		for (std::size_t i = 0; i < cookie_params.size(); ++i)
		{
		}


	}

	cookie::~cookie()
	{
	}

	void cookie::get_cookie()
	{
	}

	void cookie::set_cookie()
	{

	}

	void cookie::delete_cookie()
	{

	}

} // namespace waspp
