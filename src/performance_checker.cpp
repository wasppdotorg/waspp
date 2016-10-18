/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <chrono>

#include "performance_checker.hpp"
#include "logger.hpp"

namespace waspp
{

	performance_checker::performance_checker(int64_t limit_ms_, const char* file_, int line_) : limit_ms(limit_ms_), file(file_), line(line_)
	{
		start = std::chrono::system_clock::now();
	}

	performance_checker::~performance_checker()
	{
		int64_t diff_ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();

		if (diff_ms > limit_ms)
		{
			log(warn) << "performance_checker: " << diff_ms << " ms," << file << ":" << line;
		}
	}

} // namespace waspp
