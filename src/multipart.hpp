/*
Copyright (c) 2014-2016 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_MULTIPART_HPP
#define WASPP_MULTIPART_HPP

#include <string>

namespace waspp
{

	struct multipart_header
	{
		multipart_header()
		{
		}

		multipart_header(const std::string& disposition_, const std::string& name_, const std::string& filename_, const std::string& filetype_) : disposition(disposition_), name(name_), filename(filename_), filetype(filetype_)
		{
		}

		std::string disposition;
		std::string name;
		std::string filename;
		std::string filetype;
	};

	struct multipart_content
	{
		multipart_content()
		{
		}

		multipart_content(const std::string& name_, const std::string& filename_, const std::string& filetype_, const std::string& data_) : name(name_), filename(filename_), filetype(filetype_), data(data_)
		{
		}

		std::string name;
		std::string filename;
		std::string filetype;
		std::string data;
	};

} // namespace waspp

#endif // WASPP_MULTIPART_HPP
