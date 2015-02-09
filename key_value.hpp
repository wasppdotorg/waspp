//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_KEY_VALUE_HPP
#define WASPP_KEY_VALUE_HPP

#include <string>

namespace waspp
{

	struct key_value
	{
		key_value()
		{
		}

		key_value(std::string key_, std::string value_) : key(key_), value(value_)
		{
		}

		bool compare_key(const std::string& key_)
		{
			return key == key_;
		}

		std::string key;
		std::string value;
	};

} // namespace waspp

#endif // WASPP_KEY_VALUE_HPP
