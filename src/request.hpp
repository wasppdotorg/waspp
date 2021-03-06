//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef request_hpp
#define request_hpp

#include <vector>

#include "name_value.hpp"
#include "multipart.hpp"

namespace waspp
{

	/// A request received from a client.
	struct request
	{
		void parse_connection_header();

		std::string& header(const std::string& name);
		std::string& cookie(const std::string& name);
		std::string& param(const std::string& name);

		std::string remote_addr;
		unsigned short int remote_port;
		char connection_option;

		std::string method;
		std::string uri;
		int http_version_major_;
		int http_version_minor_;
		std::vector<name_value> headers;

		std::string content;
		int content_length;

		std::vector<name_value> cookies;
		std::vector<name_value> params;
		std::vector<std::string> rest_params;
		std::vector<multipart_content> uploads;

	};

} // namespace waspp

#endif // request_hpp
