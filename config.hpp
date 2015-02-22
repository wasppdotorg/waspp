//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_CONFIG_HPP
#define WASPP_CONFIG_HPP

#include "singleton.hpp"

namespace waspp
{

	class config
		: public singleton<config>
	{
	public:
		config();
		~config();

		bool init(const char* file, const char* server_id);
		std::map<std::string, std::string>* get(const std::string& item);

		std::string log_level, log_rotation;

		std::string address, port, doc_root;
		std::size_t num_threads;

	private:
		std::map< std::string, std::map<std::string, std::string> > cfg_;

	};

} // namespace waspp

#endif // WASPP_CONFIG_HPP
