//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_REQUEST_HPP
#define WASPP_REQUEST_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <boost/bind.hpp>
#include "key_value.hpp"

namespace waspp
{

	/// A request received from a client.
	struct request
	{
		std::string get_header(std::string key)
		{
			std::vector<key_value>::iterator found;
			found = std::find_if(headers.begin(), headers.end(), boost::bind(&key_value::compare_key, _1, key));

			if (found == headers.end())
			{
				return found->value;
			}

			return std::string();
		}

		std::string get_param(std::string key)
		{
			std::vector<key_value>::iterator found;
			found = std::find_if(params.begin(), params.end(), boost::bind(&key_value::compare_key, _1, key));

			if (found == params.end())
			{
				return found->value;
			}

			return std::string();
		}

		std::string method;
		std::string uri;
		int http_version_major;
		int http_version_minor;
		std::vector<key_value> headers;

		std::size_t content_length;
		std::string content;

		std::vector<key_value> params;
	};

} // namespace waspp

#endif // WASPP_REQUEST_HPP
