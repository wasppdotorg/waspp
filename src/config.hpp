/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_CONFIG_HPP
#define WASPP_CONFIG_HPP

#include <string>
#include <vector>

#include <boost/unordered_map.hpp>

#include "error.hpp"
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
		const std::string& log_level() { return log_level_; }
		const std::string& log_rotation() { return log_rotation_; }
		int flush_threshold() { return flush_threshold_; }

		// error message
		const std::string& err_msg(error_type err_code);

		// access
		const std::vector<std::string> access_granted() { return access_granted_; }
		const std::vector<std::string> access_denied() { return access_denied_; }

		// session
		const std::string& encrypt_key() { return encrypt_key_; }
		const std::string& sess_cookie() { return sess_cookie_; }
		double expiry_sec() { return expiry_sec_; }
		double update_sec() { return update_sec_; }
		bool validate_ip() { return validate_ip_; }
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
		std::string log_level_, log_rotation_;
		int flush_threshold_;

		// access
		std::vector<std::string> access_granted_, access_denied_;

		// session
		std::string encrypt_key_, sess_cookie_;
		double expiry_sec_, update_sec_;
		bool validate_ip_, validate_ua_;

		// server
		std::string address_, port_, doc_root_;
		std::size_t num_threads_;
		bool compress_;

		// ssl
		bool ssl_;
		std::string ssl_crt_, ssl_key_;

		// cfg
		boost::unordered_map< std::string, boost::unordered_map<std::string, std::string> > cfg_;

		// error message
		std::string locale;
		boost::unordered_map<int, std::string> err_;

	};

} // namespace waspp

#endif // WASPP_CONFIG_HPP
