//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_MD5_HPP
#define WASPP_MD5_HPP

#include <string>
#include <openssl/md5.h>

namespace waspp
{

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

} // namespace waspp

#endif // WASPP_MD5_HPP
