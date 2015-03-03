//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_REQUEST_PARSER_HPP
#define WASPP_REQUEST_PARSER_HPP

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

#include "multipart.hpp"

namespace waspp
{

	struct request;

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
			while (begin != end)
			{
				boost::tribool result = consume(req, *begin++);
				if (result || !result)
				{
					return boost::make_tuple(result, begin);
				}
			}

			boost::tribool result = boost::indeterminate;
			return boost::make_tuple(result, begin);
		}

		void decode_params(request& req);

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
		/// Handle the next character of input.
		boost::tribool consume(request& req, char input);

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
