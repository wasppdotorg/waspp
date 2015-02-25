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
#include "key_value.hpp"

namespace waspp
{

	class cookie
	{
	public:
		cookie(request* req_);
		~cookie();

		void set_cookie();
		void get_cookie();
		void delete_cookie();

	private:
		request* req;
		std::map<std::string, std::string> data_;

	};

} // namespace waspp

#endif // WASPP_COOKIE_HPP
