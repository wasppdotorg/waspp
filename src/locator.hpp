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
		static void init(T* location)
		{
			location_ = location;
		}

		static T& location()
		{
			return *location_;
		}

	protected:
		locator() {}
		virtual ~locator() {}

	private:
		static T* location_;

	};

	template<typename T> T* locator<T>::location_ = nullptr;

} // namespace waspp

#endif // locator_hpp
