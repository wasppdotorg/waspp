//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <boost/lexical_cast.hpp>

#include "config.hpp"

namespace waspp
{

	config::config()
	{
	}

	config::~config()
	{
	}

	bool config::init(const char* file, const char* item)
	{
		try
		{
			std::ifstream is(file, std::ios::in | std::ios::binary);
			if (!is)
			{
				std::cerr << "config::file not found" << std::endl;
				return false;
			}

			boost::property_tree::ptree pt;
			read_json(file, pt);

			BOOST_FOREACH(boost::property_tree::ptree::value_type const& item_, pt.get_child(""))
			{
				std::map<std::string, std::string> i;
				BOOST_FOREACH(boost::property_tree::ptree::value_type const& pair_, item_.second)
				{
					i.insert(std::make_pair(pair_.first, pair_.second.get_value<std::string>()));
				}

				c.insert(std::make_pair(item_.first, i));
			}

			std::map< std::string, std::map<std::string, std::string> >::iterator found;

			found = c.find("log");
			if (found == c.end())
			{
				std::cerr << "config::log not found" << std::endl;
				return false;
			}

			if ((found->second).find("level") == (found->second).end() ||
				(found->second).find("rotation") == (found->second).end() ||
				(found->second).find("file") == (found->second).end())
			{
				std::cerr << "config::element not found" << std::endl;
				return false;
			}

			log_level = c["log"]["level"];
			log_rotation = c["log"]["rotation"];
			log_file = c["log"]["file"];

			found = c.find(item);
			if (found == c.end())
			{
				std::cerr << "config::item not found" << std::endl;
				return false;
			}

			if ((found->second).find("address") == (found->second).end() ||
				(found->second).find("port") == (found->second).end() ||
				(found->second).find("doc_root") == (found->second).end() ||
				(found->second).find("num_threads") == (found->second).end())
			{
				std::cerr << "config::element not found" << std::endl;
				return false;
			}

			address = c[item]["address"];
			port = c[item]["port"];
			doc_root = c[item]["doc_root"];
			num_threads = boost::lexical_cast<std::size_t>(c[item]["num_threads"]);

			return true;
		}
		catch (std::exception& e)
		{
			std::cerr << "exception: " << e.what() << "\n";
		}

		return false;
	}

	std::map<std::string, std::string> config::get(const std::string& item)
	{
		std::map< std::string, std::map<std::string, std::string> >::iterator found;
		found = c.find(item);

		if (found != c.end())
		{
			return found->second;
		}

		return std::map<std::string, std::string>();
	}

} // namespace waspp
