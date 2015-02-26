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
		cookie(request* req_, response* res_);
		~cookie();

		void set_cookie(const std::string& name, const std::string& value);
		std::string& get_cookie(const std::string& name);
		void delete_cookie(const std::string& name);

	private:
		request* req;
		response* res;

	};

} // namespace waspp

#endif // WASPP_COOKIE_HPP
