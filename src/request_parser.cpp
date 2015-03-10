//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cctype>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "request_parser.hpp"
#include "request.hpp"
#include "utility.hpp"

namespace waspp
{

	request_parser::request_parser()
		: state_(method_start)
	{
	}

	void request_parser::reset()
	{
		state_ = method_start;
	}

	void request_parser::parse_params(request& req)
	{
		std::string request_uri = url_decode(req.uri);

		std::vector<std::string> rest_params;
		boost::split(rest_params, request_uri, boost::is_any_of("/"));
		rest_params.erase(rest_params.begin());

		req.rest_params = rest_params;
		
		if (req.params.size() == 0)
		{
			return;
		}

		for (std::size_t i = 0; i < req.params.size(); ++i)
		{
			req.params[i].name = url_decode(req.params[i].name);
			req.params[i].value = url_decode(req.params[i].value);
		}
	}

	/* -*-mode:c++; c-file-style: "gnu";-*- */
	/*
	*  $Id: CgiEnvironment.cpp,v 1.29 2014/06/11 04:43:46 sebdiaz Exp $
	*
	*  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
	*                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
	*  Part of the GNU cgicc library, http://www.gnu.org/software/cgicc
	*
	*  This library is free software; you can redistribute it and/or
	*  modify it under the terms of the GNU Lesser General Public
	*  License as published by the Free Software Foundation; either
	*  version 3 of the License, or (at your option) any later version.
	*
	*  This library is distributed in the hope that it will be useful,
	*  but WITHOUT ANY WARRANTY; without even the implied warranty of
	*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	*  Lesser General Public License for more details.
	*
	*  You should have received a copy of the GNU Lesser General Public
	*  License along with this library; if not, write to the Free Software
	*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
	*
	*/

	void request_parser::parse_cookies(request& req)
	{
		std::string data = req.header("Cookie");
		if (data.empty())
		{
			return;
		}

		std::string::size_type pos;
		std::string::size_type old_pos = 0;

		while (true)
		{
			pos = data.find(";", old_pos);
			if (pos == std::string::npos)
			{
				parse_cookie(req, data.substr(old_pos));
				return;
			}

			parse_cookie(req, data.substr(old_pos, pos - old_pos));
			old_pos = pos + 1;
		}
	}

	void request_parser::parse_cookie(request& req, const std::string& data)
	{
		std::string::size_type pos = data.find("=", 0);
		if (pos == std::string::npos)
		{
			return;
		}

		std::string::size_type space_count = 0;
		std::string::const_iterator data_iter;

		for (data_iter = data.begin(); data_iter != data.end(); ++data_iter, ++space_count)
		{
			if (std::isspace(*data_iter) == 0)
			{
				break;
			}
		}

		std::string name = data.substr(space_count, pos - space_count);
		std::string value = data.substr(++pos);

		req.cookies.push_back(name_value(name, value));
	}

	/* -*-mode:c++; c-file-style: "gnu";-*- */
	/*
	*  $Id: Cgicc.cpp,v 1.34 2014/04/23 20:55:04 sebdiaz Exp $
	*
	*  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
	*                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
	*  Part of the GNU cgicc library, http://www.gnu.org/software/cgicc
	*
	*  This library is free software; you can redistribute it and/or
	*  modify it under the terms of the GNU Lesser General Public
	*  License as published by the Free Software Foundation; either
	*  version 3 of the License, or (at your option) any later version.
	*
	*  This library is distributed in the hope that it will be useful,
	*  but WITHOUT ANY WARRANTY; without even the implied warranty of
	*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	*  Lesser General Public License for more details.
	*
	*  You should have received a copy of the GNU Lesser General Public
	*  License along with this library; if not, write to the Free Software
	*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
	*/

	void request_parser::parse_content(request& req)
	{
		if (req.content.empty())
		{
			return;
		}

		std::string standard_type = "application/x-www-form-urlencoded";
		std::string multipart_type = "multipart/form-data";

		std::string content_type = req.header("Content-Type");
		if (content_type.empty() || strings_are_equal(content_type, standard_type, standard_type.size()))
		{
			std::string name, value;
			std::string::size_type pos;
			std::string::size_type old_pos = 0;

			while (true)
			{
				pos = req.content.find_first_of("&=", old_pos);
				if (pos == std::string::npos)
				{
					break;
				}

				if (req.content.at(pos) == '&')
				{
					const char* c_str_ = req.content.c_str() + old_pos;
					while (*c_str_ == '&')
					{
						++c_str_;
						++old_pos;
					}

					if (old_pos >= pos)
					{
						old_pos = ++pos;
						continue;
					}

					name = url_decode(req.content.substr(old_pos, pos - old_pos));
					req.params.push_back(name_value(name, std::string()));
					old_pos = ++pos;
					continue;
				}

				name = url_decode(req.content.substr(old_pos, pos - old_pos));
				old_pos = ++pos;

				pos = req.content.find_first_of(";&", old_pos);
				value = url_decode(req.content.substr(old_pos, pos - old_pos));
				req.params.push_back(name_value(name, value));

				if (pos == std::string::npos)
				{
					break;
				}

				old_pos = ++pos;
			}
		}
		else if (strings_are_equal(multipart_type, content_type, multipart_type.size()))
		{
			std::string b_type = "boundary=";
			std::string::size_type pos = content_type.find(b_type);

			std::string sep1 = content_type.substr(pos + b_type.size());
			if (sep1.find(";") != std::string::npos)
			{
				sep1 = sep1.substr(0, sep1.find(";"));
			}
			sep1.append("\r\n");
			sep1.insert(0, "--");

			std::string sep2 = content_type.substr(pos + b_type.size());
			if (sep2.find(";") != std::string::npos)
			{
				sep2 = sep2.substr(0, sep2.find(";"));
			}
			sep2.append("--\r\n");
			sep2.insert(0, "--");

			std::string::size_type start = req.content.find(sep1);
			std::string::size_type sep_size = sep1.size();
			std::string::size_type old_pos = start + sep_size;

			while (true)
			{
				pos = req.content.find(sep1, old_pos);
				if (pos == std::string::npos)
				{
					break;
				}

				parse_multipart_content(req, req.content.substr(old_pos, pos - old_pos));
				old_pos = pos + sep_size;
			}

			pos = req.content.find(sep2, old_pos);
			if (pos != std::string::npos)
			{
				parse_multipart_content(req, req.content.substr(old_pos, pos - old_pos));
			}
		}
	}

	multipart_header request_parser::parse_multipart_header(request& req, const std::string& data)
	{
		std::string disposition;
		disposition = extract_between(data, "Content-Disposition: ", ";");

		std::string name;
		name = extract_between(data, "name=\"", "\"");

		std::string filename;
		filename = extract_between(data, "filename=\"", "\"");

		std::string c_type;
		c_type = extract_between(data, "Content-Type: ", "\r\n\r\n");

		filename = url_decode(filename);

		return multipart_header(disposition, name, filename, c_type);
	}

	void request_parser::parse_multipart_content(request& req, const std::string& data)
	{
		std::string end = "\r\n\r\n";

		std::string::size_type head_limit = data.find(end, 0);
		if (head_limit == std::string::npos)
		{
			throw std::runtime_error("invalid part header");
		}

		std::string::size_type value_start = head_limit + end.size();
		std::string value = data.substr(value_start, data.size() - value_start - 2);

		multipart_header head = parse_multipart_header(req, data.substr(0, value_start));

		if (head.filename.empty())
		{
			req.params.push_back(name_value(head.name, value));
		}
		else
		{
			req.uploads.push_back(multipart_content(head.name, head.filename, head.filetype, value));
		}
	}

	boost::tribool request_parser::consume(request& req, char input)
	{
		switch (state_)
		{
		case method_start:
			if (!is_char(input) || is_ctl(input) || is_tspecial(input))
			{
				return false;
			}
			else
			{
				state_ = method;
				req.method.push_back(input);
				return boost::indeterminate;
			}
		case method:
			if (input == ' ')
			{
				state_ = uri;
				return boost::indeterminate;
			}
			else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
			{
				return false;
			}
			else
			{
				req.method.push_back(input);
				return boost::indeterminate;
			}
		case uri:
			if (input == '?')
			{
				state_ = param_name_start;
				req.uri.push_back(input);
				return boost::indeterminate;
			}
			else if (input == ' ')
			{
				state_ = http_version_h;
				return boost::indeterminate;
			}
			else if (is_ctl(input))
			{
				return false;
			}
			else
			{
				req.uri.push_back(input);
				return boost::indeterminate;
			}
		case param_name_start:
			if (input == ' ')
			{
				state_ = http_version_h;
				return boost::indeterminate;
			}
			else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
			{
				return false;
			}
			else
			{
				state_ = param_name;
				req.uri.push_back(input);
				req.params.push_back(name_value());
				req.params.back().name.push_back(input);
				return boost::indeterminate;
			}
		case param_name:
			if (input == ' ')
			{
				state_ = http_version_h;
				return boost::indeterminate;
			}
			else if (input == '=')
			{
				state_ = param_value;
				req.uri.push_back(input);
				return boost::indeterminate;
			}
			else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
			{
				return false;
			}
			else
			{
				req.uri.push_back(input);
				req.params.back().name.push_back(input);
				return boost::indeterminate;
			}
		case param_value:
			if (input == ' ')
			{
				state_ = http_version_h;
				return boost::indeterminate;
			}
			else if (input == '&')
			{
				state_ = param_name_start;
				req.uri.push_back(input);
				return boost::indeterminate;
			}
			else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
			{
				return false;
			}
			else
			{
				req.uri.push_back(input);
				req.params.back().value.push_back(input);
				return boost::indeterminate;
			}
		case http_version_h:
			if (input == 'H')
			{
				state_ = http_version_t_1;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case http_version_t_1:
			if (input == 'T')
			{
				state_ = http_version_t_2;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case http_version_t_2:
			if (input == 'T')
			{
				state_ = http_version_p;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case http_version_p:
			if (input == 'P')
			{
				state_ = http_version_slash;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case http_version_slash:
			if (input == '/')
			{
				req.http_version_major = 0;
				req.http_version_minor = 0;
				state_ = http_version_major_start;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case http_version_major_start:
			if (is_digit(input))
			{
				req.http_version_major = req.http_version_major * 10 + input - '0';
				state_ = http_version_major;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case http_version_major:
			if (input == '.')
			{
				state_ = http_version_minor_start;
				return boost::indeterminate;
			}
			else if (is_digit(input))
			{
				req.http_version_major = req.http_version_major * 10 + input - '0';
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case http_version_minor_start:
			if (is_digit(input))
			{
				req.http_version_minor = req.http_version_minor * 10 + input - '0';
				state_ = http_version_minor;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case http_version_minor:
			if (input == '\r')
			{
				state_ = expecting_newline_1;
				return boost::indeterminate;
			}
			else if (is_digit(input))
			{
				req.http_version_minor = req.http_version_minor * 10 + input - '0';
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case expecting_newline_1:
			if (input == '\n')
			{
				state_ = header_line_start;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case header_line_start:
			if (input == '\r')
			{
				state_ = expecting_newline_3;
				return boost::indeterminate;
			}
			else if (!req.headers.empty() && (input == ' ' || input == '\t'))
			{
				state_ = header_lws;
				return boost::indeterminate;
			}
			else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
			{
				return false;
			}
			else
			{
				req.headers.push_back(name_value());
				req.headers.back().name.push_back(input);
				state_ = header_name;
				return boost::indeterminate;
			}
		case header_lws:
			if (input == '\r')
			{
				state_ = expecting_newline_2;
				return boost::indeterminate;
			}
			else if (input == ' ' || input == '\t')
			{
				return boost::indeterminate;
			}
			else if (is_ctl(input))
			{
				return false;
			}
			else
			{
				state_ = header_value;
				req.headers.back().value.push_back(input);
				return boost::indeterminate;
			}
		case header_name:
			if (input == ':')
			{
				state_ = space_before_header_value;
				return boost::indeterminate;
			}
			else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
			{
				return false;
			}
			else
			{
				req.headers.back().name.push_back(input);
				return boost::indeterminate;
			}
		case space_before_header_value:
			if (input == ' ')
			{
				state_ = header_value;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case header_value:
			if (input == '\r')
			{
				state_ = expecting_newline_2;
				return boost::indeterminate;
			}
			else if (is_ctl(input))
			{
				return false;
			}
			else
			{
				req.headers.back().value.push_back(input);
				return boost::indeterminate;
			}
		case expecting_newline_2:
			if (input == '\n')
			{
				state_ = header_line_start;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case expecting_newline_3:
			if (req.method == "GET" && input == '\n')
			{
				return true;
			}
			else if (req.method == "POST" && input == '\n')
			{
				req.content_length = 0;

				std::string content_length_str = req.header("Content-Length");
				if (content_length_str.empty())
				{
					return false;
				}
				req.content_length = boost::lexical_cast<std::size_t>(content_length_str);

				state_ = content_start;
				return boost::indeterminate;
			}
			else
			{
				return false;
			}
		case content_start:
			if (!req.content.empty())
			{
				return false;
			}
			else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
			{
				return false;
			}
			else
			{
				req.content.push_back(input);
				if (req.content.size() == req.content_length)
				{
					return true;
				}

				state_ = content;
				return boost::indeterminate;
			}
		case content:
			req.content.push_back(input);
			if (req.content.size() == req.content_length)
			{
				return true;
			}

			return boost::indeterminate;
		default:
			return false;
		}
	}

	bool request_parser::is_char(int c)
	{
		return c >= 0 && c <= 127;
	}

	bool request_parser::is_ctl(int c)
	{
		return (c >= 0 && c <= 31) || (c == 127);
	}

	bool request_parser::is_tspecial(int c)
	{
		switch (c)
		{
		case '(':
		case ')':
		case '<':
		case '>':
		case '@':
		case ',':
		case ';':
		case ':':
		case '\\':
		case '"':
		case '/':
		case '[':
		case ']':
		case '?':
		case '=':
		case '{':
		case '}':
		case ' ':
		case '\t':
			return true;
		default:
			return false;
		}
	}

	bool request_parser::is_digit(int c)
	{
		return c >= '0' && c <= '9';
	}

} // namespace waspp

