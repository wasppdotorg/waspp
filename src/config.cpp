//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <map>
#include <string>

#include <boost/property_tree/ptree.hpp>

// when using both boost asio and boost ptree on win32
// put these lines below before json_parser.hpp
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <boost/lexical_cast.hpp>

#include "logger.hpp"
#include "config.hpp"

namespace waspp
{

	config::config()
	{
	}

	config::~config()
	{
	}

	bool config::init(const std::string& file, const std::string& server_id)
	{
		logger* log = logger::instance();

		try
		{
			std::ifstream is(file.c_str(), std::ios::in | std::ios::binary);
			if (!is)
			{
				log->fatal("config::file not found");
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

				cfg_.insert(std::make_pair(item_.first, i));
			}

			std::vector<std::string> keys;
			std::map< std::string, std::map<std::string, std::string> >::iterator found;
			
			found = cfg_.find("log");
			{
				if (found == cfg_.end())
				{
					log->fatal("config::log not found");
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("level");
					keys.push_back("rotation");
				}

				for (std::size_t i = 0; i < keys.size(); ++i)
				{
					if ((found->second).find(keys[i]) == (found->second).end())
					{
						log->fatal("config::element not found");
						return false;
					}
				}

				level = cfg_.at("log").at("level");
				rotation = cfg_.at("log").at("rotation");
			}

			found = cfg_.find("session");
			{
				if (found == cfg_.end())
				{
					log->fatal("config::session not found");
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("encrypt_key");
					keys.push_back("cookie_name");
					keys.push_back("expiry_sec");
					keys.push_back("update_sec");
					keys.push_back("validate_ip_addr");
					keys.push_back("validate_u_agent");
				}

				for (std::size_t i = 0; i < keys.size(); ++i)
				{
					if ((found->second).find(keys[i]) == (found->second).end())
					{
						log->fatal("config::element not found");
						return false;
					}
				}

				encrypt_key = cfg_.at("session").at("encrypt_key");
				cookie_name = cfg_.at("session").at("cookie_name");
				expiry_sec = boost::lexical_cast<double>(cfg_.at("session").at("expiry_sec"));
				update_sec = boost::lexical_cast<double>(cfg_.at("session").at("update_sec"));
				validate_ip_addr = boost::lexical_cast<bool>(cfg_.at("session").at("validate_ip_addr"));
				validate_u_agent = boost::lexical_cast<bool>(cfg_.at("session").at("validate_u_agent"));
			}
			
			found = cfg_.find(server_id);
			{
				if (found == cfg_.end())
				{
					log->fatal("config::server_id not found");
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("address");
					keys.push_back("port");
					keys.push_back("doc_root");
					keys.push_back("num_threads");
				}

				for (std::size_t i = 0; i < keys.size(); ++i)
				{
					if ((found->second).find(keys[i]) == (found->second).end())
					{
						log->fatal("config::element not found");
						return false;
					}
				}

				address = cfg_.at(server_id).at("address");
				port = cfg_.at(server_id).at("port");
				doc_root = cfg_.at(server_id).at("doc_root");
				num_threads = boost::lexical_cast<std::size_t>(cfg_.at(server_id).at("num_threads"));
			}
			
			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

	std::map<std::string, std::string>& config::get(const std::string& item)
	{
		std::map< std::string, std::map<std::string, std::string> >::iterator found;
		found = cfg_.find(item);

		if (found == cfg_.end())
		{
			throw std::runtime_error("config::get failed");
		}

		return found->second;
	}

} // namespace waspp
