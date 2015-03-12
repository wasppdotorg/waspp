/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_JSONP_STATUS_HPP
#define WASPP_JSONP_STATUS_HPP

#include <string>

namespace waspp
{
	
	const std::string jsonp_start = "_(";
	const std::string jsonp_end = ");";

	enum jsonp_status_type
	{
		status_okay = 2000,
		status_unauthorized = 4001,
		status_error = 9999
	};

	namespace jsonp_status
	{

		const std::string& str(jsonp_status_type status_code);

	} // namespace jsonp_status

} // namespace waspp

#endif // WASPP_JSONP_STATUS_HPP
