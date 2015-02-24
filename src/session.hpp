//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_SESSION_HPP
#define WASPP_SESSION_HPP

#include <string>

namespace waspp
{

	class session
	{
	public:
		session();
		~session();

	private:
		bool load();

		std::string get_sess();
		void set_sess();

		void create();
		void update();
		void destory();

		std::string encrypt_key;
		std::string cookie_name;
		int expiry_time;
		int update_time;

	};

} // namespace waspp

#endif // WASPP_SESSION_HPP
