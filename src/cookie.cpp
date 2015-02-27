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

	cookie::cookie(request* req_, response* res_) : req(req_), res(res_)
	{
		std::string cookie = req->get_header("Cookie");
		if (cookie.empty())
		{
			return;
		}

		std::vector<std::string> cookies;
		boost::split(cookies, cookie, boost::is_any_of(";"));

		std::size_t last_pos;
		for (std::size_t i = 0; i < cookies.size(); ++i)
		{
			boost::algorithm::trim(cookies[i]);

			last_pos = cookies[i].find_last_of("=");
			if (last_pos == std::string::npos)
			{
				continue;
			}

			req->cookie.insert(std::make_pair(cookies[i].substr(0, last_pos), cookies[i].substr(last_pos + 1)));
		}
	}

	cookie::~cookie()
	{
	}

	std::string& cookie::get_cookie(const std::string& name)
	{
		return req->cookie[name];
	}

	void cookie::set_cookie(const std::string& name, const std::string& value)
	{
		res->cookie[name] = value;
	}

	void cookie::delete_cookie(const std::string& name)
	{
		set_cookie(name, std::string());
	}

} // namespace waspp
