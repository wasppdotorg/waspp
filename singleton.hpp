//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_SINGLETON_HPP
#define WASPP_SINGLETON_HPP

#include <cstdlib>

#include <boost/atomic.hpp>
#include <boost/thread.hpp>

namespace waspp
{

	template<typename T>
	class singleton
	{
	public:
		static T* instance()
		{
			if(instance_ == 0)
			{
				boost::lock_guard<boost::mutex> lock(mutex_);
				if(instance_ == 0)
				{
					instance_ = new T();
					atexit(destory);
				}
			}

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

		static boost::atomic<T*> instance_;
		static boost::mutex mutex_;

	};

	template<typename T> boost::atomic<T*> singleton<T>::instance_ = 0;
	template<typename T> boost::mutex singleton<T>::mutex_;

} // namespace waspp

#endif // WASPP_SINGLETON_HPP
