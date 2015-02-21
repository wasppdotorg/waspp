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

		bool init(const std::vector<std::string>& dbnames);
		
		dbconn_ptr get(const std::string& dbname);
		void free(const std::string& dbname, dbconn_ptr dbconn);

	private:
		std::map<std::string, dbconn_pool*> db_;

	};

} // namespace waspp

#endif // WASPP_DATABASE_HPP
