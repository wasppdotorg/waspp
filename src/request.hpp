//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_REQUEST_HPP
#define WASPP_REQUEST_HPP

#include <map>
#include <vector>
#include <string>
#include <algorithm>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

#include "name_value.hpp"

namespace waspp
{

	/// A request received from a client.
	struct request
	{
		std::string get_header(const std::string& name);
		std::string& get_cookie(const std::string& name);
		std::string get_param(const std::string& name);

		void parse_cookie();
		void parse_content();

		std::string remote_addr;
		std::string method;
		std::string uri;
		int http_version_major;
		int http_version_minor;
		std::vector<name_value> headers;

		std::string content;
		std::size_t content_length;

		std::map<std::string, std::string> cookies;
		std::vector<name_value> params;
	};

} // namespace waspp

#endif // WASPP_REQUEST_HPP
