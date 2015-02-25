//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>
#include <algorithm>
#include <string>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include "request.hpp"
#include "response.hpp"
#include "key_value.hpp"
#include "cookie.hpp"

namespace waspp
{

	cookie::cookie(request* req_, response* res_) : req(req_), res(res_)
	{
		std::vector<key_value>::iterator found;
		found = std::find_if(req->headers.begin(), req->headers.end(), boost::bind(&key_value::compare_key, _1, "Cookie"));

		if (found == req->headers.end())
		{
			return;
		}

		std::vector<std::string> cookie_params;
		boost::split(cookie_params, found->value, boost::is_any_of("; "));

		std::size_t last_pos;
		for (std::size_t i = 0; i < cookie_params.size(); ++i)
		{
			last_pos = cookie_params[i].find_last_of("=");
			if (last_pos == std::string::npos)
			{
				continue;
			}

			data_.insert(std::make_pair(cookie_params[i].substr(0, last_pos), cookie_params[i].substr(last_pos + 1)));
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
