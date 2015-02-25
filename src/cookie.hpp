//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_COOKIE_HPP
#define WASPP_COOKIE_HPP

#include <map>
#include <vector>
#include <string>

#include "request.hpp"
#include "response.hpp"
#include "key_value.hpp"

namespace waspp
{

	class cookie
	{
	public:
		cookie(request& req);
		~cookie();

		void set_cookie(const std::string& name, const std::string& value);
		std::string& get_cookie(const std::string& name);
		void delete_cookie(const std::string& name);

		std::map<std::string, std::string>& all();
		std::string str(std::map<std::string, std::string>::iterator i);

	private:
		std::map<std::string, std::string> cookie_;

	};

} // namespace waspp

#endif // WASPP_COOKIE_HPP
