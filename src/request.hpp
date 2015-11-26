//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_REQUEST_HPP
#define WASPP_REQUEST_HPP

#include <string>
#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/bind.hpp>

#include "name_value.hpp"
#include "multipart.hpp"

namespace waspp
{

	/// A request received from a client.
	struct request
	{
		void parse_remote_endpoint(const std::string& remote_endpoint);

		std::string& header(const std::string& name);
		std::string& cookie(const std::string& name);
		std::string& param(const std::string& name);

		void reset();

		std::string remote_addr;
		std::string remote_port;
		char connection_option;

		std::string method;
		std::string uri;
		int http_version_major;
		int http_version_minor;
		std::vector<name_value> headers;

		std::string content;
		int content_length;

		std::vector<name_value> cookies;
		std::vector<name_value> params;
		std::vector<std::string> rest_params;
		std::vector<multipart_content> uploads;

		std::locale loc;

	};

} // namespace waspp

#endif // WASPP_REQUEST_HPP
