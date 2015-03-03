//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_PART_HEADER_HPP
#define WASPP_PART_HEADER_HPP

#include <string>

namespace waspp
{

	struct part_header
	{
		part_header()
		{
		}

		part_header(const std::string& disposition_, const std::string& name_, const std::string& filename_, const std::string& filetype_) : disposition(disposition_), name(name_), filename(filename_), filetype(filetype_)
		{
		}

		std::string disposition;
		std::string name;
		std::string filename;
		std::string filetype;
	};

} // namespace waspp

#endif // WASPP_PART_HEADER_HPP
