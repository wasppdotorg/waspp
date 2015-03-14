/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef WASPP_CONFIG_HPP
#define WASPP_CONFIG_HPP

#include <string>
#include <vector>

#include "name_value.hpp"
#include "app_status.hpp"
#include "utility.hpp"

namespace waspp
{

	struct cfgpair
	{
		cfgpair(const std::string& first_, std::vector<name_value> second_) : first(first_), second(second_)
		{
		}

		bool compare_first(const std::string& first_)
		{
			return first == first_;
		}

		std::string first;
		std::vector<name_value> second;
	};

	struct statuspair
	{
		statuspair(int first_, const std::string& second_) : first(first_), second(second_)
		{
		}

		bool compare_first(int first_)
		{
			return first == first_;
		}

		int first;
		std::string second;
	};

	class config
		: public singleton<config>
	{
	public:
		config();
		~config();

		bool init(const std::string& file, const std::string& server_id);
		std::vector<name_value>& get(const std::string& item);
		const std::string& msg(app_status_type status_code);

		// logger
		std::string level, rotation;

		// session
		std::string encrypt_key, sess_cookie;
		double expiry_sec, update_sec;
		bool validate_ep, validate_ua;

		// server
		std::string address, port, doc_root;
		std::size_t num_threads;

	private:
		std::vector<cfgpair> cfg_;

		// status messages
		std::string msg_locale;
		std::vector<statuspair> status_;

	};

} // namespace waspp

#endif // WASPP_CONFIG_HPP
