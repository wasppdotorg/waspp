/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_FUNCTION_TIMER_HPP
#define WASPP_FUNCTION_TIMER_HPP

#include <cstdint>

#include <boost/chrono.hpp>

#include "logger.hpp"

namespace waspp
{

	class function_timer
	{
	public:
		function_timer(boost::int_least64_t limit_ms, const char* file_, int line_);
		~function_timer();

	private:
		function_timer();

		boost::int_least64_t limit_ms;
		const char* file;
		int line;

		boost::chrono::system_clock::time_point start;

	};

} // namespace waspp

#endif // WASPP_FUNCTION_TIMER_HPP
