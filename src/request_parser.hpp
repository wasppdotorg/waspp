//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_REQUEST_PARSER_HPP
#define WASPP_REQUEST_PARSER_HPP

#include <boost/tuple/tuple.hpp>
#include <boost/logic/tribool.hpp>
#include <boost/lexical_cast.hpp>

#include "request.hpp"
#include "multipart.hpp"

namespace waspp
{

	/// Parser for incoming requests.
	class request_parser
	{
	public:
		/// Construct ready to parse the request method.
		request_parser();

		/// Reset to initial parser state.
		void reset();

		/// Parse some data. The tribool return value is true when a complete request
		/// has been parsed, false if the data is invalid, indeterminate when more
		/// data is required. The InputIterator return value indicates how much of the
		/// input has been consumed.
		template <typename InputIterator>
		boost::tuple<boost::tribool, InputIterator> parse(request& req,
			InputIterator begin, InputIterator end)
		{
			char input;
			boost::tribool result = boost::indeterminate;

			while (begin != end)
			{
				switch (state_)
				{
				case method_start:
					input = *begin++;

					if (!is_char(input) || is_ctl(input) || is_tspecial(input))
					{
						result = false;
						break;
					}
					else
					{
						state_ = method;
						req.method.push_back(input);
						result = boost::indeterminate;
						break;
					}

				case method:
					do
					{
						input = *begin++;

						if (input == ' ')
						{
							state_ = uri;
							result = boost::indeterminate;
							break;
						}
						else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
						{
							result = false;
							break;
						}
						else
						{
							req.method.push_back(input);
						}

					} while (begin != end);
					break;

				case uri:
					do
					{
						input = *begin++;

						if (input == '?')
						{
							state_ = param_name_start;
							req.uri.push_back(input);
							result = boost::indeterminate;
							break;
						}
						else if (input == ' ')
						{
							state_ = http_version_h;
							result = boost::indeterminate;
							break;
						}
						else if (is_ctl(input))
						{
							result = false;
							break;
						}
						else
						{
							req.uri.push_back(input);
						}

					} while (begin != end);
					break;

				case param_name_start:
					input = *begin++;

					if (input == ' ')
					{
						state_ = http_version_h;
						result = boost::indeterminate;
						break;
					}
					else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
					{
						result = false;
						break;
					}
					else
					{
						state_ = param_name;
						req.uri.push_back(input);
						req.params.push_back(name_value());
						req.params.back().name.push_back(input);
						result = boost::indeterminate;
						break;
					}

				case param_name:
					do
					{
						input = *begin++;

						if (input == ' ')
						{
							state_ = http_version_h;
							result = boost::indeterminate;
							break;
						}
						else if (input == '=')
						{
							state_ = param_value;
							req.uri.push_back(input);
							result = boost::indeterminate;
							break;
						}
						else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
						{
							result = false;
							break;
						}
						else
						{
							req.uri.push_back(input);
							req.params.back().name.push_back(input);
						}

					} while (begin != end);
					break;

				case param_value:
					do
					{
						input = *begin++;

						if (input == ' ')
						{
							state_ = http_version_h;
							result = boost::indeterminate;
							break;
						}
						else if (input == '&')
						{
							state_ = param_name_start;
							req.uri.push_back(input);
							result = boost::indeterminate;
							break;
						}
						else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
						{
							result = false;
							break;
						}
						else
						{
							req.uri.push_back(input);
							req.params.back().value.push_back(input);
						}

					} while (begin != end);
					break;

				case http_version_h:
					do
					{
						input = *begin++;

						if (input == 'H')
						{
							state_ = http_version_t_1;
							result = boost::indeterminate;
							break;
						}
						else
						{
							result = false;
							break;
						}

					} while (begin != end);
					break;


				case http_version_t_1:
					input = *begin++;

					if (input == 'T')
					{
						state_ = http_version_t_2;
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case http_version_t_2:
					input = *begin++;

					if (input == 'T')
					{
						state_ = http_version_p;
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case http_version_p:
					input = *begin++;

					if (input == 'P')
					{
						state_ = http_version_slash;
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case http_version_slash:
					input = *begin++;

					if (input == '/')
					{
						req.http_version_major_ = 0;
						req.http_version_minor_ = 0;
						state_ = http_version_major_start;
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case http_version_major_start:
					input = *begin++;

					if (is_digit(input))
					{
						req.http_version_major_ = req.http_version_major_ * 10 + input - '0';
						state_ = http_version_major;
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case http_version_major:
					input = *begin++;

					if (input == '.')
					{
						state_ = http_version_minor_start;
						result = boost::indeterminate;
						break;
					}
					else if (is_digit(input))
					{
						req.http_version_major_ = req.http_version_major_ * 10 + input - '0';
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case http_version_minor_start:
					input = *begin++;

					if (is_digit(input))
					{
						req.http_version_minor_ = req.http_version_minor_ * 10 + input - '0';
						state_ = http_version_minor;
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case http_version_minor:
					input = *begin++;

					if (input == '\r')
					{
						state_ = expecting_newline_1;
						result = boost::indeterminate;
						break;
					}
					else if (is_digit(input))
					{
						req.http_version_minor_ = req.http_version_minor_ * 10 + input - '0';
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case expecting_newline_1:
					input = *begin++;

					if (input == '\n')
					{
						state_ = header_line_start;
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case header_line_start:
					input = *begin++;

					if (input == '\r')
					{
						state_ = expecting_newline_3;
						result = boost::indeterminate;
						break;
					}
					else if (!req.headers.empty() && (input == ' ' || input == '\t'))
					{
						state_ = header_lws;
						result = boost::indeterminate;
						break;
					}
					else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
					{
						result = false;
						break;
					}
					else
					{
						req.headers.push_back(name_value());
						req.headers.back().name.push_back(input);
						state_ = header_name;
						result = boost::indeterminate;
						break;
					}

				case header_lws:
					input = *begin++;

					if (input == '\r')
					{
						state_ = expecting_newline_2;
						result = boost::indeterminate;
						break;
					}
					else if (input == ' ' || input == '\t')
					{
						result = boost::indeterminate;
						break;
					}
					else if (is_ctl(input))
					{
						result = false;
						break;
					}
					else
					{
						state_ = header_value;
						req.headers.back().value.push_back(input);
						result = boost::indeterminate;
						break;
					}

				case header_name:
					do
					{
						input = *begin++;

						if (input == ':')
						{
							state_ = space_before_header_value;
							result = boost::indeterminate;
							break;
						}
						else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
						{
							result = false;
							break;
						}
						else
						{
							req.headers.back().name.push_back(input);
						}

					} while (begin != end);
					break;

				case space_before_header_value:
					input = *begin++;

					if (input == ' ')
					{
						state_ = header_value;
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case header_value:
					do
					{
						input = *begin++;

						if (input == '\r')
						{
							state_ = expecting_newline_2;
							result = boost::indeterminate;
							break;
						}
						else if (is_ctl(input))
						{
							result = false;
							break;
						}
						else
						{
							req.headers.back().value.push_back(input);
						}

					} while (begin != end);
					break;

				case expecting_newline_2:
					input = *begin++;

					if (input == '\n')
					{
						state_ = header_line_start;
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case expecting_newline_3:
					input = *begin++;

					if (req.method == "GET" && input == '\n')
					{
						result = true;
						break;
					}
					else if (req.method == "POST" && input == '\n')
					{
						if (req.header("Content-Length").empty())
						{
							result = false;
							break;
						}
						req.content_length = boost::lexical_cast<int>(req.header("Content-Length"));

						state_ = content_start;
						result = boost::indeterminate;
						break;
					}
					else
					{
						result = false;
						break;
					}

				case content_start:
					input = *begin++;

					if (!req.content.empty())
					{
						result = false;
						break;
					}
					else if (!is_char(input) || is_ctl(input) || is_tspecial(input))
					{
						result = false;
						break;
					}
					else
					{
						--req.content_length;
						req.content.push_back(input);
						if (req.content_length == 0)
						{
							req.content_length = boost::lexical_cast<int>(req.header("Content-Length"));
							result = true;
							break;
						}

						state_ = content;
						result = boost::indeterminate;
						break;
					}

				case content:
					while (1)
					{
						input = *begin++;

						--req.content_length;
						req.content.push_back(input);
						if (req.content_length == 0)
						{
							req.content_length = boost::lexical_cast<int>(req.header("Content-Length"));
							result = true;
							break;
						}

						result = boost::indeterminate;
						break;

					}
					break;

				default:
					result = false;
					break;
				}

				if (result || !result)
				{
					return boost::make_tuple(result, begin);
				}
			}

			return boost::make_tuple(result, begin);
		}

		void parse_params(request& req);

		/* -*-mode:c++; c-file-style: "gnu";-*- */
		/*
		*  $Id: CgiEnvironment.h,v 1.21 2014/04/23 20:55:03 sebdiaz Exp $
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

		void parse_cookies(request& req);
		void parse_cookie(request& req, const std::string& data);

		/* -*-mode:c++; c-file-style: "gnu";-*- */
		/*
		*  $Id: Cgicc.h,v 1.20 2014/04/23 20:55:04 sebdiaz Exp $
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

		void parse_content(request& req);

		multipart_header parse_multipart_header(request& req, const std::string& data);
		void parse_multipart_content(request& req, const std::string& data);

	private:
		/// Check if a byte is an HTTP character.
		static bool is_char(int c);

		/// Check if a byte is an HTTP control character.
		static bool is_ctl(int c);

		/// Check if a byte is defined as an HTTP tspecial character.
		static bool is_tspecial(int c);

		/// Check if a byte is a digit.
		static bool is_digit(int c);

		/// The current state of the parser.
		enum state
		{
			method_start,
			method,
			uri,
			param_name_start,
			param_name,
			param_value_start,
			param_value,
			http_version_h,
			http_version_t_1,
			http_version_t_2,
			http_version_p,
			http_version_slash,
			http_version_major_start,
			http_version_major,
			http_version_minor_start,
			http_version_minor,
			expecting_newline_1,
			header_line_start,
			header_lws,
			header_name,
			space_before_header_value,
			header_value,
			expecting_newline_2,
			expecting_newline_3,
			content_start,
			content
		} state_;
	};

} // namespace waspp

#endif // WASPP_REQUEST_PARSER_HPP
