/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_CONFIG_HPP
#define WASPP_CONFIG_HPP

#include <string>
#include <vector>

#include <boost/unordered_map.hpp>

#include "status.hpp"
#include "utility.hpp"

namespace waspp
{

	class config
		: public singleton<config>
	{
	public:
		config();
		~config();

		bool init(const std::string& file, const std::string& server_id);
		boost::unordered_map<std::string, std::string>& get(const std::string& item);

		// logger
		const std::string& level() { return level_; }
		const std::string& rotation() { return rotation_; }

		// message
		const std::string& msg(status_type status_code);

		// session
		const std::string& encrypt_key() { return encrypt_key_; }
		const std::string& sess_cookie() { return sess_cookie_; }
		double expiry_sec() { return expiry_sec_; }
		double update_sec() { return update_sec_; }
		bool validate_ep() { return validate_ep_; }
		bool validate_ua() { return validate_ua_; }

		// server
		const std::string& address() { return address_; }
		const std::string& port() { return port_; }
		const std::string& doc_root() { return doc_root_; }
		std::size_t num_threads() { return num_threads_; }
		bool compress() { return compress_; }

		// ssl
		bool ssl() { return ssl_; }
		const std::string& ssl_crt() { return ssl_crt_; }
		const std::string& ssl_key() { return ssl_key_; }

	private:
		// logger
		std::string level_, rotation_;

		// session
		std::string encrypt_key_, sess_cookie_;
		double expiry_sec_, update_sec_;
		bool validate_ep_, validate_ua_;

		// server
		std::string address_, port_, doc_root_;
		std::size_t num_threads_;
		bool compress_;

		// ssl
		bool ssl_;
		std::string ssl_crt_, ssl_key_;

		// cfg
		boost::unordered_map< std::string, boost::unordered_map<std::string, std::string> > cfg_;

		// status messages
		std::string msg_locale;
		boost::unordered_map<int, std::string> status_;

	};

} // namespace waspp

#endif // WASPP_CONFIG_HPP
