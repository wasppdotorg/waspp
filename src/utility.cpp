/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <cctype>

#include <string>
#include <exception>

#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>

#include "utility.hpp"

namespace waspp
{

	uri_conn::uri_conn(uri_request_type req_type_, const std::string& host_, const std::string& port_) : req_type(req_type_), host(host_), port(port_), io_service_(), resolver_(io_service_)
	{
		if (port.empty())
		{
			if (req_type == http_get || req_type == http_post)
			{
				port = "http";
			}
			else if (req_type == https_get || req_type == https_post)
			{
				port = "https";
			}
		}
	}

	uri_conn::~uri_conn()
	{
	}

	void uri_conn::set_http_headers(const std::vector<name_value>& req_headers_)
	{
		req_headers = req_headers_;
	}

	bool uri_conn::query(const std::string& uri, const std::string& data)
	{
		try
		{
			if (req_type == tcp || req_type == http_get || req_type == http_post)
			{
				// Get a list of endpoints corresponding to the server name.
				boost::asio::ip::tcp::resolver::query query_(host, port);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver_.resolve(query_);

				// Try each endpoint until we successfully establish a connection.
				boost::asio::ip::tcp::socket socket_(io_service_);
				boost::asio::connect(socket_, endpoint_iterator);

				// Form the request. We specify the "Connection: close" header so that the
				// server will close the socket after transmitting the response. This will
				// allow us to treat all data up until the EOF as the content.
				std::ostream req_stream(&req_buf);

				if (req_type == tcp)
				{
					return tcp_query(socket_, req_stream, data);
				}
				else if (req_type == http_get)
				{
					set_http_get(req_stream, uri);
					return http_query(socket_);
				}
				else if (req_type == http_post)
				{
					set_http_post(req_stream, uri, data);
					return http_query(socket_);
				}
				else
				{
					return false;
				}

				return true;
			}
			else if (req_type == ssl || req_type == https_get || req_type == https_post)
			{
#ifdef _SSL
				// Get a list of endpoints corresponding to the server name.
				boost::asio::ip::tcp::resolver::query query_(host, port);
				boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver_.resolve(query_);

				boost::asio::ssl::context context_(io_service_, boost::asio::ssl::context::sslv23);
				context_.set_verify_mode(boost::asio::ssl::verify_none);

				// Try each endpoint until we successfully establish a connection.
				boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_(io_service_, context_);

				boost::asio::connect(socket_.lowest_layer(), endpoint_iterator);
				socket_.handshake(boost::asio::ssl::stream_base::client);

				// Form the request. We specify the "Connection: close" header so that the
				// server will close the socket after transmitting the response. This will
				// allow us to treat all data up until the EOF as the content.
				std::ostream req_stream(&req_buf);

				if (req_type == ssl)
				{
					return ssl_query(socket_, req_stream, data);
				}
				else if (req_type == https_get)
				{
					set_http_get(req_stream, uri);
					return https_query(socket_);
				}
				else if (req_type == https_post)
				{
					set_http_post(req_stream, uri, data);
					return https_query(socket_);
				}
				else
				{
					return false;
				}

				return true;
#endif // SSL
			}
		}
		catch (...)
		{

		}

		return false;
	}

	bool uri_conn::query(const std::string& uri)
	{
		return query(uri, std::string());
	}

	bool uri_conn::query()
	{
		return query(std::string());
	}

	const std::string& uri_conn::res_headers()
	{
		return res_headers_;
	}

	const std::string& uri_conn::res_content()
	{
		return res_content_;
	}

	void uri_conn::set_http_get(std::ostream& req_stream, const std::string& uri)
	{
		req_stream << "GET " << uri << " HTTP/1.1\r\n";
		req_stream << "Host: " << host << "\r\n";
		req_stream << "Accept: */*\r\n";
		req_stream << "Connection: close\r\n";

		for (std::size_t i = 0; i < req_headers.size(); ++i)
		{
			req_stream << req_headers[i].name << ": ";
			req_stream << req_headers[i].value << "\r\n";
		}

		req_stream << "\r\n";
	}

	void uri_conn::set_http_post(std::ostream& req_stream, const std::string& uri, const std::string& data)
	{
		req_stream << "POST " << uri << " HTTP/1.1\r\n";
		req_stream << "Host: " << host << "\r\n";
		req_stream << "Accept: */*\r\n";
		req_stream << "Connection: close\r\n";

		for (std::size_t i = 0; i < req_headers.size(); ++i)
		{
			req_stream << req_headers[i].name << ": ";
			req_stream << req_headers[i].value << "\r\n";
		}

		req_stream << "\r\n";
		req_stream << data;
	}

	bool uri_conn::tcp_query(boost::asio::ip::tcp::socket& socket_, std::ostream& req_stream, const std::string& data)
	{
		try
		{
			if (!data.empty())
			{
				req_stream << data;

				// Send the request.
				boost::asio::write(socket_, req_buf);
			}

			boost::system::error_code error;
			boost::asio::read(socket_, res_buf, boost::asio::transfer_at_least(1), error);

			if (error)
			{
				return false;
			}

			std::ostringstream oss;
			oss << &res_buf;

			res_content_ = oss.str();
			return true;
		}
		catch (...)
		{

		}

		return false;
	}

	bool uri_conn::http_query(boost::asio::ip::tcp::socket& socket_)
	{
		try
		{
			// Send the request.
			boost::asio::write(socket_, req_buf);

			// Read the response status line. The response streambuf will automatically
			// grow to accommodate the entire line. The growth may be limited by passing
			// a maximum size to the streambuf constructor.
			boost::asio::read_until(socket_, res_buf, "\r\n");

			// Check that response is OK.
			std::istream res_stream(&res_buf);
			if (!is_200(res_stream))
			{
				return false;
			}

			// Read the response headers, which are terminated by a blank line.
			boost::asio::read_until(socket_, res_buf, "\r\n\r\n");

			// Process the response headers.
			while (std::getline(res_stream, res_headers_) && res_headers_ != "\r")
			{
			}

			// Write whatever content we already have to output.
			std::ostringstream oss;
			if (res_buf.size() > 0)
			{
				oss << &res_buf;
			}

			// Read until EOF, writing data to output as we go.
			boost::system::error_code error;
			while (boost::asio::read(socket_, res_buf,
				boost::asio::transfer_at_least(1), error))
			{
				oss << &res_buf;
			}

			if (error != boost::asio::error::eof)
			{
				throw boost::system::system_error(error);
			}

			res_content_ = oss.str();
			return true;
		}
		catch (...)
		{

		}

		return false;
	}

#ifdef _SSL
	bool uri_conn::ssl_query(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket_, std::ostream& req_stream, const std::string& data)
	{
		try
		{
			if (!data.empty())
			{
				req_stream << data;

				// Send the request.
				boost::asio::write(socket_, req_buf);
			}

			boost::system::error_code error;
			boost::asio::read(socket_, res_buf, boost::asio::transfer_at_least(1), error);

			if (error)
			{
				return false;
			}

			std::ostringstream oss;
			oss << &res_buf;

			res_content_ = oss.str();
			return true;
		}
		catch (...)
		{

		}

		return false;
	}

	bool uri_conn::https_query(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket_)
	{
		try
		{
			// Send the request.
			boost::asio::write(socket_, req_buf);

			// Read the response status line. The response streambuf will automatically
			// grow to accommodate the entire line. The growth may be limited by passing
			// a maximum size to the streambuf constructor.
			boost::asio::read_until(socket_, res_buf, "\r\n");

			// Check that response is OK.
			std::istream res_stream(&res_buf);
			if (!is_200(res_stream))
			{
				return false;
			}

			// Read the response headers, which are terminated by a blank line.
			boost::asio::read_until(socket_, res_buf, "\r\n\r\n");

			// Process the response headers.
			while (std::getline(res_stream, res_headers_) && res_headers_ != "\r")
			{
			}

			// Write whatever content we already have to output.
			std::ostringstream oss;
			if (res_buf.size() > 0)
			{
				oss << &res_buf;
			}

			// Read until EOF, writing data to output as we go.
			boost::system::error_code error;
			while (boost::asio::read(socket_, res_buf,
				boost::asio::transfer_at_least(1), error))
			{
				oss << &res_buf;
			}

			if (error != boost::asio::error::eof)
			{
				throw boost::system::system_error(error);
			}

			res_content_ = oss.str();
			return true;
		}
		catch (...)
		{

		}

		return false;
	}
#endif // SSL

	bool uri_conn::is_200(std::istream& res_stream)
	{
		std::string http_version;
		res_stream >> http_version;

		unsigned int status_code;
		res_stream >> status_code;

		std::string status_msg;
		std::getline(res_stream, status_msg);

		if (!res_stream || http_version.substr(0, 5) != "HTTP/")
		{
			return false;
		}

		if (status_code != 200)
		{
			return false;
		}

		return true;
	}

	std::string get_extension(const std::string& path)
	{
		// Determine the file extension.
		std::size_t last_slash_pos = path.find_last_of("/");
		std::size_t last_dot_pos = path.find_last_of(".");

		if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos)
		{
			return path.substr(last_dot_pos + 1);
		}

		return std::string();
	}

	std::string md5_digest(const std::string& str)
	{
		unsigned char digest[16] = { 0 };
		char* c_str = const_cast<char*>(str.c_str());

		MD5_CTX ctx;
		MD5_Init(&ctx);
		MD5_Update(&ctx, c_str, strlen(c_str));
		MD5_Final(digest, &ctx);

		char md_str[33];
		for (int i = 0; i < 16; ++i)
		{
			sprintf(&md_str[i * 2], "%02x", (unsigned int)digest[i]);
		}

		return std::string(md_str);
	}

	void gzip_str(std::string& str)
	{
		std::stringstream original;
		original << str;

		boost::iostreams::filtering_streambuf<boost::iostreams::input> buf;
		buf.push(boost::iostreams::gzip_compressor(boost::iostreams::zlib::best_speed));
		buf.push(original);

		std::stringstream gzipped;
		boost::iostreams::copy(buf, gzipped);

		str = gzipped.str();
	}

	/* -*-mode:c++; c-file-style: "gnu";-*- */
	/*
	*  $Id: CgiUtils.cpp,v 1.20 2014/04/23 20:55:03 sebdiaz Exp $
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

	// case-insensitive string comparison
	// This code based on code from
	// "The C++ Programming Language, Third Edition" by Bjarne Stroustrup
	bool strings_are_equal(const std::string& s1, const std::string& s2)
	{
		std::string::const_iterator p1 = s1.begin();
		std::string::const_iterator p2 = s2.begin();
		std::string::const_iterator l1 = s1.end();
		std::string::const_iterator l2 = s2.end();

		while (p1 != l1 && p2 != l2)
		{
			if (std::toupper(*(p1++)) != std::toupper(*(p2++)))
			{
				return false;
			}
		}

		return (s2.size() == s1.size()) ? true : false;
	}

	// case-insensitive string comparison
	bool strings_are_equal(const std::string& s1, const std::string& s2, std::size_t n)
	{
		std::string::const_iterator p1 = s1.begin();
		std::string::const_iterator p2 = s2.begin();
		bool good = (n <= s1.length() && n <= s2.length());
		std::string::const_iterator l1 = good ? (s1.begin() + n) : s1.end();
		std::string::const_iterator l2 = good ? (s2.begin() + n) : s2.end();

		while (p1 != l1 && p2 != l2)
		{
			if (std::toupper(*(p1++)) != std::toupper(*(p2++)))
			{
				return false;
			}
		}

		return good;
	}

	std::string extract_between(const std::string& data, const std::string& separator1, const std::string& separator2)
	{
		std::string result;
		std::string::size_type start, limit;

		start = data.find(separator1, 0);
		if (std::string::npos != start)
		{
			start += separator1.length();
			limit = data.find(separator2, start);
			if (std::string::npos != limit)
			{
				result = data.substr(start, limit - start);
			}
		}

		return result;
	}

	std::string extract_between(const std::string& datas, const std::string& separators)
	{
		return extract_between(datas, separators, separators);
	}

	std::string char2hex(char c)
	{
		std::string result;
		char first, second;

		first = (c & 0xF0) / 16;
		first += first > 9 ? 'A' - 10 : '0';
		second = c & 0x0F;
		second += second > 9 ? 'A' - 10 : '0';

		result.append(1, first);
		result.append(1, second);

		return result;
	}

	char hex2char(char first, char second)
	{
		int digit;

		digit = (first >= 'A' ? ((first & 0xDF) - 'A') + 10 : (first - '0'));
		digit *= 16;
		digit += (second >= 'A' ? ((second & 0xDF) - 'A') + 10 : (second - '0'));

		return static_cast<char>(digit);
	}

	std::string percent_encode(const std::string& src)
	{
		std::string result;
		std::string::const_iterator iter;

		for (iter = src.begin(); iter != src.end(); ++iter)
		{
			switch (*iter)
			{
			case ' ':
				//result.append(1, '+');
				result.append("%20");
				break;
				// alnum
			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
			case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
			case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
			case 'V': case 'W': case 'X': case 'Y': case 'Z':
			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
			case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
			case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
			case 'v': case 'w': case 'x': case 'y': case 'z':
			case '0': case '1': case '2': case '3': case '4': case '5': case '6':
			case '7': case '8': case '9':
				// mark
			case '-': case '_': case '.': case '!': case '~': case '*': case '\'':
			case '(': case ')':
				result.append(1, *iter);
				break;
				// escape
			default:
				result.append(1, '%');
				result.append(char2hex(*iter));
				break;
			}
		}

		return result;
	}

	std::string percent_decode(const std::string& src)
	{
		std::string result;
		std::string::const_iterator iter;
		char c;

		for (iter = src.begin(); iter != src.end(); ++iter)
		{
			switch (*iter)
			{
			case '+':
				result.append(1, ' ');
				break;
			case '%':
				// Don't assume well-formed input
				if (std::distance(iter, src.end()) >= 2 &&
					std::isxdigit(*(iter + 1)) &&
					std::isxdigit(*(iter + 2)))
				{
					c = *++iter;
					result.append(1, hex2char(c, *++iter));
				}
				// Just pass the % through untouched
				else
				{
					result.append(1, '%');
				}
				break;

			default:
				result.append(1, *iter);
				break;
			}
		}

		return result;
	}

	/*
	base64.cpp and base64.h

	Copyright (C) 2004-2008 Rene Nyffenegger

	This source code is provided 'as-is', without any express or implied
	warranty. In no event will the author be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this source code must not be misrepresented; you must not
	claim that you wrote the original source code. If you use this source code
	in a product, an acknowledgment in the product documentation would be
	appreciated but is not required.

	2. Altered source versions must be plainly marked as such, and must not be
	misrepresented as being the original source code.

	3. This notice may not be removed or altered from any source distribution.

	Rene Nyffenegger rene.nyffenegger@adp-gmbh.ch
	*/

	// this is NOT the original source code.
	// updated the function base64_encode to get string input.

	std::string base64_encode(const std::string& string_to_encode)
	{
		const unsigned char* bytes_to_encode = reinterpret_cast<const unsigned char*>(string_to_encode.c_str());
		unsigned int in_len = string_to_encode.size();

		std::string ret;
		int i = 0;
		int j = 0;
		unsigned char char_array_3[3];
		unsigned char char_array_4[4];

		while (in_len--)
		{
			char_array_3[i++] = *(bytes_to_encode++);
			if (i == 3)
			{
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; i < 4; ++i)
				{
					ret += base64_chars[char_array_4[i]];
				}
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 3; ++j)
			{
				char_array_3[j] = '\0';
			}

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; j < (i + 1); ++j)
			{
				ret += base64_chars[char_array_4[j]];
			}

			while ((i++ < 3))
			{
				ret += '=';
			}
		}

		return ret;
	}

	std::string base64_decode(std::string const& encoded_string)
	{
		int in_len = encoded_string.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		unsigned char char_array_4[4], char_array_3[3];
		std::string ret;

		while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
		{
			char_array_4[i++] = encoded_string[in_];
			++in_;

			if (i == 4)
			{
				for (i = 0; i < 4; ++i)
				{
					char_array_4[i] = base64_chars.find(char_array_4[i]);
				}

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; i < 3; ++i)
				{
					ret += char_array_3[i];
				}
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 4; ++j)
			{
				char_array_4[j] = 0;
			}

			for (j = 0; j < 4; j++)
			{
				char_array_4[j] = base64_chars.find(char_array_4[j]);
			}

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; j < (i - 1); ++j)
			{
				ret += char_array_3[j];
			}
		}

		return ret;
	}

} // namespace waspp

