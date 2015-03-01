/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_UTILITY_HPP
#define WASPP_UTILITY_HPP

#include <cstdio>
#include <cstdlib>

#include <openssl/md5.h>

#include <vector>
#include <string>

#include <boost/atomic.hpp>
#include <boost/thread.hpp>

namespace waspp
{

	namespace utility
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
			singleton() {};
			virtual ~singleton() {};

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

		class md5
		{
		public:
			md5() {};
			~md5() {};

			std::string digest(const std::string& str_)
			{
				unsigned char digest_[16];
				char* c_str_ = const_cast<char*>(str_.c_str());

				MD5_CTX ctx;
				MD5_Init(&ctx);
				MD5_Update(&ctx, c_str_, strlen(c_str_));
				MD5_Final(digest_, &ctx);

				char md_str[33];
				for (int i = 0; i < 16; ++i)
				{
					sprintf(&md_str[i * 2], "%02x", (unsigned int)digest_[i]);
				}

				return std::string(md_str);
			}

		private:

		};

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
		// changed the function name from base64_encode to __base64_encode_impl
		// and wrapped it with the function which has only one parameter.

		static const std::string base64_chars = 
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";

		static inline bool __is_base64(unsigned char c)
		{
			return (isalnum(c) || (c == '+') || (c == '/'));
		}

		std::string __base64_encode_impl(unsigned char const* bytes_to_encode, unsigned int in_len);

		std::string base64_encode(std::string const& string_to_encode);
		std::string base64_decode(std::string const& encoded_string);

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

		std::string __char2hex(char c);
		char __hex2char(char first, char second);

		std::string url_encode(const std::string& src);
		std::string url_decode(const std::string& src);

	} // namespace utility

} // namespace waspp

#endif // WASPP_UTILITY_HPP
