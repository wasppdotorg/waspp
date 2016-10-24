//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cctype>

#include <exception>
#include <sstream>

#include "request_parser.hpp"
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
		auto request_uri = percent_decode(req.uri);

		split(request_uri, '/', req.rest_params);
		req.rest_params.erase(req.rest_params.begin());

		if (req.params.size() == 0)
		{
			return;
		}

		for (auto& req_param : req.params)
		{
			req_param.name = percent_decode(req_param.name);
			req_param.value = percent_decode(req_param.value);
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

					name = percent_decode(req.content.substr(old_pos, pos - old_pos));
					req.params.push_back(name_value(name, std::string()));
					old_pos = ++pos;
					continue;
				}

				name = percent_decode(req.content.substr(old_pos, pos - old_pos));
				old_pos = ++pos;

				pos = req.content.find_first_of(";&", old_pos);
				value = percent_decode(req.content.substr(old_pos, pos - old_pos));
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

		filename = percent_decode(filename);

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

	void request_parser::split(const std::string& s, char c, std::vector<std::string>& v)
	{
		std::stringstream ss;
		ss.str(s);

		std::string item;
		while (std::getline(ss, item, c))
		{
			v.push_back(item);
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

