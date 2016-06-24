/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef singleton_hpp
#define singleton_hpp

namespace waspp
{

	template<typename T>
	class singleton
	{
	public:
		static T* instance()
		{
			if (instance_ == nullptr)
			{
				instance_ = new T();

				// avoid memory leak
				atexit(destroy);
			}

			return instance_;
		}

	protected:
		singleton() {}
		virtual ~singleton() {}

	private:
		static void destroy()
		{
			delete instance_;
		}

		static T* instance_;

	};

	template<typename T> T* singleton<T>::instance_ = nullptr;

} // namespace waspp

#endif // singleton_hpp
