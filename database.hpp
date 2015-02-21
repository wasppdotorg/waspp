//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_DATABASE_HPP
#define WASPP_DATABASE_HPP

#include <map>
#include <vector>
#include <string>

#include "singleton.hpp"
#include "dbconn_pool.hpp"

namespace waspp
{

	class database
		: public singleton<database>
	{
	public:
		database();
		~database();

		void add(const std::pair<std::string, dbconn_pool*>& pair_);
		bool init();

	private:
		std::map<std::string, dbconn_pool*> db_;

	};

} // namespace waspp

#endif // WASPP_DATABASE_HPP
