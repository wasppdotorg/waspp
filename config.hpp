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

		bool load(const char* file, const char* item);

		std::string address, port, doc_root;
		std::size_t num_threads;
		std::map< std::string, std::map<std::string, std::string> > c;

	private:

	};

} // namespace waspp

#endif // WASPP_CONFIG_HPP
