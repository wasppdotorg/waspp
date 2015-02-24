//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>
#include <algorithm>

#include <boost/bind.hpp>

#include "key_value.hpp"
#include "session.hpp"

namespace waspp
{

	session::session(std::vector<key_value>& headers)
	{
		if (!load(headers))
		{
			create();
		}
		else
		{
			update();
		}
	}

	session::~session()
	{
	}

	bool session::load(std::vector<key_value>& headers)
	{
		std::vector<key_value>::iterator found;

		found = std::find_if(headers.begin(), headers.end(), boost::bind(&key_value::compare_key, _1, "Set-Cookie"));
		if (found == headers.end())
		{
			return false;
		}

		found = std::find_if(headers.begin(), headers.end(), boost::bind(&key_value::compare_key, _1, "User-Agent"));
		std::string user_agent;
		if (found != headers.end())
		{
			user_agent.append(found->value);
		}

		for (std::size_t i = 0; i < headers.size(); ++i)
		{

		}

		return false;
	}

	std::string session::get_sess() { return std::string(); }
	void session::set_sess() {}

	void session::create() {}
	void session::update() {}
	void session::destory() {}

} // namespace waspp
