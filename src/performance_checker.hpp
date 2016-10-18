/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef performance_checker_hpp
#define performance_checker_hpp

#include <cstdint>

#include <chrono>

namespace waspp
{

	class performance_checker
	{
	public:
		performance_checker(int64_t limit_ms, const char* file_, int line_);
		~performance_checker();

	private:
		performance_checker();

		int64_t limit_ms;
		const char* file;
		int line;

		std::chrono::system_clock::time_point start;

	};

} // namespace waspp

#endif // performance_checker_hpp
