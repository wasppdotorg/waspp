/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <ctime>

#include <map>
#include <vector>
#include <string>

#include <sstream>
#include <algorithm>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/uuid/uuid.hpp>

#include "logger.hpp"
#include "config.hpp"
#include "request.hpp"
#include "response.hpp"
#include "utility.hpp"
#include "session.hpp"

namespace waspp
{

	session::session(logger* log_, config* cfg_, request* req_, response* res_) : log(log_), cfg(cfg_), req(req_), res(res_)
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
			std::string session_cookie = req->cookie(cfg->sess_cookie);
			if (session_cookie.empty())
			{
				return false;
			}

			std::string session_md5 = session_cookie.substr(session_cookie.size() - 32);
			std::string session_str = session_cookie.substr(0, session_cookie.size() - 32);

			if (session_md5 != md5_digest(session_str + cfg->encrypt_key))
			{
				res->delete_cookie(cfg->sess_cookie);
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
					res->delete_cookie(cfg->sess_cookie);
					return false;
				}
			}

			if (std::difftime(std::time(0), get_last_tm()) > cfg->expiry_sec)
			{
				res->delete_cookie(cfg->sess_cookie);
				return false;
			}

			if (cfg->validate_ip && get_curr_ip() != get_last_ip())
			{
				res->delete_cookie(cfg->sess_cookie);
				return false;
			}

			if (cfg->validate_ua && get_curr_ua() != get_last_ua())
			{
				res->delete_cookie(cfg->sess_cookie);
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
		std::string sess_id = md5_digest(get_uuid());

		session_.clear();
		session_.insert(std::make_pair("sess_id", sess_id));
		session_.insert(std::make_pair("last_tm", get_curr_tm()));
		session_.insert(std::make_pair("last_ip", get_curr_ip()));
		session_.insert(std::make_pair("last_ua", get_curr_ua()));

		serialize_and_set();
	}

	void session::update()
	{
		if (std::difftime(std::time(0), get_last_tm()) < cfg->update_sec)
		{
			return;
		}

		std::string new_sess_id(get_uuid());
		new_sess_id.append(get_curr_ip());

		session_.at("sess_id") = md5_digest(new_sess_id);
		session_.at("last_tm") = get_curr_tm();

		serialize_and_set();
	}

	std::string session::get_uuid()
	{
		boost::uuids::uuid uuid_;
		std::vector<char> uuid_vec_char(uuid_.size());
		std::copy(uuid_.begin(), uuid_.end(), uuid_vec_char.begin());

		return std::string(uuid_vec_char.begin(), uuid_vec_char.end());
	}

	std::string session::get_curr_tm()
	{
		return boost::lexical_cast<std::string>(std::time(0));
	}

	std::time_t session::get_last_tm()
	{
		return boost::lexical_cast<std::time_t>(session_.at("last_tm"));
	}

	std::string session::get_curr_ip()
	{
		return req->remote_addr;
	}

	std::string session::get_last_ip()
	{
		return session_.at("last_ip");
	}

	std::string session::get_curr_ua()
	{
		std::string user_agent = req->header("User-Agent").substr(0, 120);
		boost::algorithm::trim(user_agent);

		return user_agent;
	}

	std::string session::get_last_ua()
	{
		return session_.at("last_ua");
	}

	void session::serialize_and_set()
	{
		// serialize
		std::ostringstream oss;
		{
			boost::archive::text_oarchive oar(oss, boost::archive::no_tracking);
			oar << session_;
		}

		std::string cookie_value(oss.str());
		cookie_value.append(md5_digest(cookie_value + cfg->encrypt_key));

		res->set_cookie(cfg->sess_cookie, cookie_value);
	}

} // namespace waspp
