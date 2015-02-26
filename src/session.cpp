//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>

#include <vector>
#include <string>
#include <algorithm>

#include <boost/bind.hpp>
#include <boost/algorithm/string.hpp>

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

		std::vector<std::string> sessions;
		boost::split(sessions, session_str, boost::is_any_of("|"));

		if (sessions.size() != 4)
		{
			return false;
		}

		std::string http_user_agent = req->get_header("User-Agent").substr(0, 120);
		boost::algorithm::trim(http_user_agent);

		return true;
	}

	std::string session::get_sess() { return std::string(); }
	void session::set_sess() {}

	void session::create() {}
	void session::update() {}
	void session::destory() {}

} // namespace waspp
