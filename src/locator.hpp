/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef locator_hpp
#define locator_hpp

namespace waspp
{

	template<typename T>
	class locator
	{
	public:
		static void init(T* inst)
		{
			instance_ = inst;
		}

		static T& instance()
		{
			return *instance_;
		}

	protected:
		locator() {}
		virtual ~locator() {}

	private:
		static T* instance_;

	};

	template<typename T> T* locator<T>::instance_ = nullptr;

} // namespace waspp

#endif // locator_hpp
