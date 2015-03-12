//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>

#include "jsonp.hpp"

namespace waspp
{

	namespace jsonp_status
	{

		const std::string okay = "okay";
		const std::string unauthorized = "unauthorized";
		const std::string error = "error";

		const std::string& str(jsonp_status_type status_code)
		{
			switch (status_code)
			{
			case status_okay:
				return okay;
			case status_unauthorized:
				return unauthorized;
			default:
				return error;
			}
		}

	} // namespace jsonp_status

} // namespace waspp

