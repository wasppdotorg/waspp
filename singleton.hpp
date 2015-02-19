//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_SINGLETON_HPP
#define WASPP_SINGLETON_HPP

namespace waspp
{

	template<typename T>
	class singleton
	{
	public:
		static T* instance()
		{
			if (!instance_)
			{
				instance_ = new T();
			}

			return instance_;
		}

	protected:
		singleton() {};
		virtual ~singleton() {};

	private:
		static T* instance_;

	};

	template<typename T> T* singleton<T>::instance_ = 0;

} // namespace waspp

#endif // WASPP_SINGLETON_HPP
