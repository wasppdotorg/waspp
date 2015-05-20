/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/chrono.hpp>

#include "function_timer.hpp"
#include "logger.hpp"

namespace waspp
{

	function_timer::function_timer(long long int limit_ms_, const char* file_, int line_) : limit_ms(limit_ms_), file(file_), line(line_)
	{
		start = boost::chrono::system_clock::now();
	}

	function_timer::~function_timer()
	{
		long long int diff_ms = boost::chrono::duration_cast<boost::chrono::milliseconds>(boost::chrono::system_clock::now() - start).count();

		if (diff_ms > limit_ms)
		{
			log(warn) << "function_timer: " << diff_ms << " ms," << file << ":" << line;
		}
	}

} // namespace waspp
