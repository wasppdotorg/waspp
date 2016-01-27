/*
Copyright (c) 2014-2016 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_UTILITY_HPP
#define WASPP_UTILITY_HPP

#include <cstdio>
#include <cstdlib>

#include <openssl/md5.h>

#include <string>
#include <vector>

#include <boost/unordered_map.hpp>
#include <boost/atomic.hpp>
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "name_value.hpp"

namespace waspp
{

	class spinlock
	{
	public:
		void acquire()
		{
			while (atomic_flag_.test_and_set(boost::memory_order_acquire))
			{
			}
		}

		void release()
		{
			atomic_flag_.clear(boost::memory_order_release);
		}

	private:
		boost::atomic_flag atomic_flag_;

	};

	template<typename T>
	class singleton
	{
	public:
		static T* instance()
		{
			if (instance_ != 0)
			{
				return instance_;
			}

			// double checked locking for thread-safe singleton
			lock.acquire();
			if (instance_ == 0)
			{
				instance_ = new T();

				// avoid memory leak
				atexit(destory);
			}
			lock.release();

			return instance_;
		}

	protected:
		singleton() {}
		virtual ~singleton() {}

	private:
		static void destory()
		{
			delete instance_;
		}

		static T* instance_;
		static spinlock lock;

	};

	template<typename T> T* singleton<T>::instance_ = 0;
	template<typename T> spinlock singleton<T>::lock;
	// end of thread-safe singleton

	enum uri_request_type
	{
		tcp = 1,
		http_get = 2,
		http_post = 3,

		ssl = 4,
		https_get = 5,
		https_post = 6
	};

	class uri_conn
	{
	public:
		uri_conn(uri_request_type req_type, const std::string& host_, const std::string& port_ = "");
		~uri_conn();

		void set_http_headers(const std::vector<name_value>& req_headers_);
		bool query(const std::string& uri, const std::string& data);
		bool query(const std::string& uri);
		bool query();

		const std::string& res_headers();
		const std::string& res_content();

	private:
		void set_http_get(std::ostream& req_stream, const std::string& uri);
		void set_http_post(std::ostream& req_stream, const std::string& uri, const std::string& data);

		bool tcp_query(boost::asio::ip::tcp::socket& socket_, std::ostream& req_stream, const std::string& data);
		bool http_query(boost::asio::ip::tcp::socket& socket_);

		bool ssl_query(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket_, std::ostream& req_stream, const std::string& data);
		bool https_query(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& socket_);

		bool is_200(std::istream& res_stream);

		uri_request_type req_type;

		std::string host;
		std::string port;

		std::vector<name_value> req_headers;

		boost::asio::io_service io_service_;
		boost::asio::ip::tcp::resolver resolver_;

		boost::asio::streambuf req_buf;
		boost::asio::streambuf res_buf;

		std::string res_headers_;
		std::string res_content_;
	};

	std::string get_extension(const std::string& path);
	std::string md5_digest(const std::string& str);

	void gzip_str(std::string& str);

	/* -*-mode:c++; c-file-style: "gnu";-*- */
	/*
	*  $Id: CgiUtils.h,v 1.17 2014/04/23 20:55:03 sebdiaz Exp $
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

	bool strings_are_equal(const std::string& s1, const std::string& s2);
	bool strings_are_equal(const std::string& s1, const std::string& s2, std::size_t n);

	std::string extract_between(const std::string& data, const std::string& separator1, const std::string& separator2);
	std::string extract_between(const std::string& datas, const std::string& separators);

	std::string char2hex(char c);
	char hex2char(char first, char second);

	// also known as url_encode
	std::string percent_encode(const std::string& src);

	// also known as url_decode
	std::string percent_decode(const std::string& src);

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

	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	static inline bool is_base64(unsigned char c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

	std::string base64_encode(std::string const& string_to_encode);
	std::string base64_decode(std::string const& encoded_string);

} // namespace waspp

#endif // WASPP_UTILITY_HPP
