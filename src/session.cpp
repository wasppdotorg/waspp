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
		cfg = config::instance();

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

	std::string& session::get_sess(const std::string& name)
	{
		return session_[name];
	}

	void session::set_sess(const std::string& name, const std::string& value)
	{
		session_[name] = value;
	}

	bool session::load()
	{
		try
		{
			std::string session_cookie = cookie_.get_cookie(cfg->sess_cookie);
			if (session_cookie.empty())
			{
				return false;
			}

			std::string session_md5 = session_cookie.substr(session_cookie.size() - 32);
			std::string session_str = session_cookie.substr(0, session_cookie.size() - 32);

			md5 md5_;
			if (session_md5 != md5_.digest(session_str + cfg->encrypt_key))
			{
				cookie_.delete_cookie(cfg->sess_cookie);
				return false;
			}

			// deserialize
			{
				std::istringstream iss(session_str);
				boost::archive::text_iarchive iar(iss, boost::archive::no_tracking);
				iar >> session_;
			}

			std::vector<std::string> keys;
			{
				keys.push_back("sess_id");
				keys.push_back("last_tm");
				keys.push_back("last_ip");
				keys.push_back("last_ua");
			}

			for (std::size_t i = 0; i < keys.size(); ++i)
			{
				if (session_.find(keys[i]) == session_.end())
				{
					cookie_.delete_cookie(cfg->sess_cookie);
					return false;
				}
			}

			if (std::difftime(get_curr_tm(), get_last_tm()) > cfg->expiry_sec)
			{
				cookie_.delete_cookie(cfg->sess_cookie);
				return false;
			}

			if (cfg->validate_ip && get_curr_ip() != get_lass_ip())
			{
				cookie_.delete_cookie(cfg->sess_cookie);
				return false;
			}

			if (cfg->validate_ua && get_curr_ua() != get_last_ua())
			{
				cookie_.delete_cookie(cfg->sess_cookie);
				return false;
			}

			return true;
		}
		catch (...)
		{
		}

		return false;
	}

	void session::create()
	{


		std::string last_tm = boost::lexical_cast<std::string>(std::time(0));

		std::string user_agent = req->get_header("User-Agent").substr(0, 120);
		boost::algorithm::trim(user_agent);

		session_.clear();
		session_.insert(std::make_pair("sess_id", uuid_str));
		session_.insert(std::make_pair("last_tm", last_tm));
		session_.insert(std::make_pair("ip_addr", req->remote_addr));
		session_.insert(std::make_pair("u_agent", user_agent));

		// serialize
		{
			std::ostringstream oss;
			boost::archive::text_oarchive oar(oss, boost::archive::no_tracking);
			oar << session_;
		}

		md5 md5_;
		std::string cookie_value(oss.str());
		cookie_value.append(md5_.digest(cookie_value + cfg->encrypt_key));

		cookie_.set_cookie(cfg->sess_cookie, cookie_value);
	}

	void session::update()
	{
		std::time_t last_tm = boost::lexical_cast<std::time_t>(session_.at("last_tm"));
		double diff = std::difftime(std::time(0), last_tm);

		if (diff < cfg->update_sec)
		{
			return;
		}

		std::string new_sess_id
	}

	std::string new_sess_id()
	{
		boost::uuids::uuid uuid_;
		std::vector<char> uuid_vec_char(uuid_.size());
		std::copy(uuid_.begin(), uuid_.end(), uuid_vec_char.begin());

		return std::string(uuid_vec_char.begin(), uuid_vec_char.end());
	}

	std::time_t get_curr_tm()
	{
		return boost::lexical_cast<std::string>(std::time(0));
	}

	std::string get_curr_tm(int time_)
	{
		return boost::lexical_cast<std::string>(std::time(0));
	}

	std::string get_last_tm()
	{
		return boost::lexical_cast<std::time_t>(session_.at("last_tm"));
	}

	std::string get_curr_ip()
	{
		return req->remote_addr;
	}

	std::string get_last_ip()
	{
		return session_.at("last_ip");
	}

	std::string get_curr_ua()
	{
		std::string user_agent = req->get_header("User-Agent").substr(0, 120);
		boost::algorithm::trim(user_agent);

		return user_agent;
	}

	std::string get_last_ua()
	{
		return session_.at("last_ua");
	}

} // namespace waspp
