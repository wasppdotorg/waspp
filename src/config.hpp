/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_CONFIG_HPP
#define WASPP_CONFIG_HPP

#include <map>
#include <string>

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
		std::map<std::string, std::string>& get(const std::string& item);

		// logger
		std::string level, rotation;

		// session
		std::string encrypt_key, sess_cookie;
		double expiry_sec, update_sec;
		bool validate_ip, validate_ua;

		// server
		std::string address, port, doc_root;
		std::size_t num_threads;

	private:
		std::map< std::string, std::map<std::string, std::string> > cfg_;

	};

} // namespace waspp

#endif // WASPP_CONFIG_HPP
