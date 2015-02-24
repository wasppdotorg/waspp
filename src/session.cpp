//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <vector>

#include "key_value.hpp"
#include "session.hpp"

namespace waspp
{

	session::session()
	{
		if (!load())
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

	bool session::load(const std::vector<key_value>& headers)
	{
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
