//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/uuid/uuid.hpp>

#include "config.hpp"
#include "request.hpp"
#include "response.hpp"
#include "cookie.hpp"
#include "md5.hpp"
#include "session.hpp"

namespace waspp
{

	session::session(request* req_, response* res_) : req(req_), cookie_(req_, res_)
	{
		if (!load())
		{
			create();
			return;
		}

		update();
	}

	session::~session()
	{
	}

	bool session::load()
	{
		config* cfg = config::instance();

		std::string session_cookie = cookie_.get_cookie(cfg->cookie_name);
		if (session_cookie.empty())
		{
			return false;
		}

		std::string session_md5 = session_cookie.substr(session_cookie.size() - 32);
		std::string session_str = session_cookie.substr(0, session_cookie.size() - 32);

		md5 md5_;
		if (session_md5 != md5_.digest(session_str + cfg->encrypt_key))
		{
			return false;
		}

		// deserialize
		{
			std::istringstream iss;
			boost::archive::text_iarchive iar(iss, 1);
			iar >> session_;
		}

		std::vector<std::string> keys;
		{
			keys.push_back("sess_id");
			keys.push_back("last_tm");
			keys.push_back("ip_addr");
			keys.push_back("u_agent");
		}

		for (std::size_t i = 0; i < keys.size(); ++i)
		{
			if (session_.data_.find(keys[i]) == session_.data_.end())
			{
				cookie_.delete_cookie(cfg->cookie_name);
				return false;
			}
		}

		std::time_t last_tm = boost::lexical_cast<std::time_t>(session_.data_.at("last_tm"));
		double diff = std::difftime(std::time(0), last_tm);

		if (diff > cfg->expiry_sec)
		{
			cookie_.delete_cookie(cfg->cookie_name);
			return false;
		}

		if (cfg->validate_ip_addr && req->remote_addr != session_.data_.at("ip_addr"))
		{
			cookie_.delete_cookie(cfg->cookie_name);
			return false;
		}

		std::string http_user_agent = req->get_header("User-Agent").substr(0, 120);
		boost::algorithm::trim(http_user_agent);

		if (cfg->validate_u_agent && http_user_agent != session_.data_.at("u_agent"))
		{
			cookie_.delete_cookie(cfg->cookie_name);
			return false;
		}

		return true;
	}

	std::string session::get_sess() { return std::string(); }
	void session::set_sess() {}

	void session::create() {}
	void session::update() {}
	void session::destory() {}

} // namespace waspp
