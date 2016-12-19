//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "locator.hpp"

namespace waspp
{

	logger* locator::logger_ = nullptr;
	config* locator::config_ = nullptr;
	database* locator::database_ = nullptr;
	redis* locator::redis_ = nullptr;


} // namespace waspp

