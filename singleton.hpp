//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_SINGLETON_HPP
#define WASPP_SINGLETON_HPP

#include <cstdlib>

#include "spinlock.hpp"

namespace waspp
{

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

			// for thread-safe singleton
			spinlock lock;
			lock.set();
			{
				if (instance_ == 0)
				{
					instance_ = new T();
					atexit(destory);
				}
			}
			lock.clear();

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

	};

	template<typename T> T* singleton<T>::instance_ = 0;

} // namespace waspp

#endif // WASPP_SINGLETON_HPP
