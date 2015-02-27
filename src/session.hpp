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

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "config.hpp"
#include "request.hpp"
#include "response.hpp"
#include "cookie.hpp"

namespace waspp
{

	class session
	{
	public:
		session(request* req_, response* res_);
		~session();

		std::string& get_sess(const std::string& name);
		void set_sess(const std::string& name, const std::string& value);

	private:
		bool load();

		void create();
		void update();

		std::string new_sess_id();

		std::string get_curr_tm();
		std::string get_last_tm();

		std::string get_curr_ip();
		std::string get_last_ip();

		std::string get_curr_ua();
		std::string get_last_ua();

		config* cfg;
		request* req;
		cookie cookie_;

		std::map<std::string, std::string> session_;

	};

} // namespace waspp

#endif // WASPP_SESSION_HPP
