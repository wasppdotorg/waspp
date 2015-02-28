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
#include "logger.hpp"
#include "request.hpp"
#include "response.hpp"

namespace waspp
{

	class session
	{
	public:
		session(config* cfg_, logger* log_, request* req_, response* res_);
		~session();

		std::string& get_sess(const std::string& name);
		void set_sess(const std::string& name, const std::string& value);

	private:
		bool load();

		void create();
		void update();

		std::string get_uuid();

		std::string get_curr_tm();
		std::time_t get_last_tm();

		std::string get_curr_ip();
		std::string get_last_ip();

		std::string get_curr_ua();
		std::string get_last_ua();

		void serialize_and_set();

		config* cfg;
		logger* log;
		request* req;
		response* res;
		utility::md5 md5_;

		std::map<std::string, std::string> session_;

	};

} // namespace waspp

#endif // WASPP_SESSION_HPP
