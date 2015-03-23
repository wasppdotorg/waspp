//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_NAME_VALUE_HPP
#define WASPP_NAME_VALUE_HPP

#include <string>

namespace waspp
{

	struct name_value
	{
		name_value()
		{
		}

		name_value(const std::string& name_, const std::string& value_) : name(name_), value(value_)
		{
		}

		bool compare_name(const std::string& name_)
		{
			return name == name_;
		}

		template<typename Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar & name;
			ar & value;
		}

		std::string name;
		std::string value;
	};

} // namespace waspp

#endif // WASPP_NAME_VALUE_HPP
