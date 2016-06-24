/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef spinlock_hpp
#define spinlock_hpp

#include <atomic>

namespace waspp
{

	class spinlock
	{
	public:
		spinlock() : state_(unlocked) {}

		void acquire()
		{
			while (state_.exchange(locked, std::memory_order_acquire) == locked)
			{
				/* busy-wait */
			}
		}

		void release()
		{
			state_.store(unlocked, std::memory_order_release);
		}

	private:
		enum state { locked, unlocked };
		std::atomic<state> state_;

	};

} // namespace waspp

#endif // spinlock_hpp
