/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_UPLOAD_HPP
#define WASPP_UPLOAD_HPP

#include <string>

namespace waspp
{

	struct upload
	{
		upload()
		{
		}

		upload(const std::string& name_, const std::string& filename_, const std::string& filetype_, const std::string& data_) : name(name_), filename(filename_), filetype(filetype_), data(data_)
		{
		}

		std::string name;
		std::string filename;
		std::string filetype;
		std::string data;
	};	

} // namespace waspp

#endif // WASPP_UPLOAD_HPP
