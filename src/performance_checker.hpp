/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_PERFORMANCE_CHECKER_HPP
#define WASPP_PERFORMANCE_CHECKER_HPP

#include <boost/chrono.hpp>

#include "logger.hpp"

namespace waspp
{

	class performance_checker
	{
	public:
		performance_checker(long long int limit_ms, const char* file_, int line_);
		~performance_checker();

	private:
		performance_checker();

		long long int limit_ms;
		const char* file;
		int line;

		boost::chrono::system_clock::time_point start;

	};

} // namespace waspp

#endif // WASPP_PERFORMANCE_CHECKER_HPP
