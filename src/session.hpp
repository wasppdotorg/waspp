/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_SESSION_HPP
#define WASPP_SESSION_HPP

#include <string>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "config.hpp"
#include "request.hpp"
#include "response.hpp"

namespace waspp
{
	// this code is based on
	// the session library of php framework called codeigniter
	// codeigniter/system/libraries/Session.php

	class session
	{
	public:
		session(config* cfg_, request* req_, response* res_);
		~session();

		std::string& get(const std::string& name);
		void put(const std::string& name, const std::string& value);

	private:
		bool load();

		void create();
		void update();

		std::string get_uuid();

		std::string get_curr_tm();
		std::time_t get_last_tm();

		std::string& get_curr_ip();
		std::string& get_last_ip();

		std::string get_curr_ua();
		std::string& get_last_ua();

		void serialize_and_set();

		config* cfg;
		request* req;
		response* res;

		std::vector<name_value> session_;

	};

} // namespace waspp

#endif // WASPP_SESSION_HPP
