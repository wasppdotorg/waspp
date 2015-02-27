//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_SPINLOCK_HPP
#define WASPP_SPINLOCK_HPP

#include <boost/atomic.hpp>
#include <boost/thread.hpp>

namespace waspp
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

} // namespace waspp

#endif // WASPP_SPINLOCK_HPP
