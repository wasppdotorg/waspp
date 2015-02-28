//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_UTILITY_HPP
#define WASPP_UTILITY_HPP

#include <cstdio>
#include <cstdlib>

#include <openssl/md5.h>

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

	} // namespace utility

} // namespace waspp

#endif // WASPP_UTILITY_HPP
