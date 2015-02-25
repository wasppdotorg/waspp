//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_SESSION_HPP
#define WASPP_SESSION_HPP

#include <map>
#include <string>

namespace waspp
{

	class session
	{
	public:
		session(std::vector<key_value>& headers);
		~session();

	private:
		bool load(std::vector<key_value>& headers);

		std::string get_sess();
		void set_sess();

		void create();
		void update();
		void destory();

		std::string encrypt_key;
		std::string cookie_name;
		double expiry_time_sec;
		double update_interval_sec;

		std::map<std::string, std::string> data_;

	};

} // namespace waspp

#endif // WASPP_SESSION_HPP
