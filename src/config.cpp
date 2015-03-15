/*
Copyright (c) 2014-2015 WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <string>

#include <boost/property_tree/ptree.hpp>

// when using both boost asio and boost ptree on win32
// put these lines below before json_parser.hpp
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#include <boost/property_tree/json_parser.hpp>
#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "config.hpp"
#include "logger.hpp"
#include "name_value.hpp"

namespace waspp
{

	config::config() : expiry_sec(0), update_sec(0), validate_ep(false), validate_ua(false), num_threads(0)
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
			if (!boost::filesystem::exists(file))
			{
				log->fatal(__FILE__, __LINE__, "config::file not found");
				return false;
			}

			boost::property_tree::ptree pt;
			read_json(file, pt);

			BOOST_FOREACH(boost::property_tree::ptree::value_type const& item_, pt.get_child(""))
			{
				std::vector<name_value> i;
				BOOST_FOREACH(boost::property_tree::ptree::value_type const& pair_, item_.second)
				{
					i.push_back(name_value(pair_.first, pair_.second.get_value<std::string>()));
				}

				cfg_.push_back(cfgpair(item_.first, i));
			}

			std::vector<std::string> keys;
			std::vector<cfgpair>::iterator found1;
			std::vector<name_value>::iterator found2;

			found1 = std::find_if(cfg_.begin(), cfg_.end(), boost::bind(&cfgpair::compare_first, _1, "log"));
			{
				if (found1 == cfg_.end())
				{
					log->fatal(__FILE__, __LINE__, "config::log not found");
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("level");
					keys.push_back("rotation");
				}

				for (std::size_t i = 0; i < keys.size(); ++i)
				{
					found2 = std::find_if(found1->second.begin(), found1->second.end(), boost::bind(&name_value::compare_name, _1, keys[i]));
					if (found2 == found1->second.end())
					{
						log->fatal(__FILE__, __LINE__, "config::element not found");
						return false;
					}

					if (keys[i] == "level")
					{
						level = found2->value;
					}
					else if (keys[i] == "rotation")
					{
						rotation = found2->value;
					}
				}
			}

			found1 = std::find_if(cfg_.begin(), cfg_.end(), boost::bind(&cfgpair::compare_first, _1, "locale"));
			{
				if (found1 == cfg_.end())
				{
					log->fatal(__FILE__, __LINE__, "config::locale not found");
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("msg_locale");
				}

				for (std::size_t i = 0; i < keys.size(); ++i)
				{
					found2 = std::find_if(found1->second.begin(), found1->second.end(), boost::bind(&name_value::compare_name, _1, keys[i]));
					if (found2 == found1->second.end())
					{
						log->fatal(__FILE__, __LINE__, "config::element not found");
						return false;
					}

					if (keys[i] == "msg_locale")
					{
						msg_locale = found2->value;
					}
				}
			}

			found1 = std::find_if(cfg_.begin(), cfg_.end(), boost::bind(&cfgpair::compare_first, _1, "session"));
			{
				if (found1 == cfg_.end())
				{
					log->fatal(__FILE__, __LINE__, "config::session not found");
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("encrypt_key");
					keys.push_back("sess_cookie");
					keys.push_back("expiry_sec");
					keys.push_back("update_sec");
					keys.push_back("validate_ep");
					keys.push_back("validate_ua");
				}

				for (std::size_t i = 0; i < keys.size(); ++i)
				{
					found2 = std::find_if(found1->second.begin(), found1->second.end(), boost::bind(&name_value::compare_name, _1, keys[i]));
					if (found2 == found1->second.end())
					{
						log->fatal(__FILE__, __LINE__, "config::element not found");
						return false;
					}

					if (keys[i] == "encrypt_key")
					{
						encrypt_key = found2->value;
					}
					else if (keys[i] == "sess_cookie")
					{
						sess_cookie = found2->value;
					}
					else if (keys[i] == "expiry_sec")
					{
						expiry_sec = boost::lexical_cast<double>(found2->value);
					}
					else if (keys[i] == "update_sec")
					{
						update_sec = boost::lexical_cast<double>(found2->value);
					}
					else if (keys[i] == "validate_ep")
					{
						validate_ep = boost::lexical_cast<bool>(found2->value);
					}
					else if (keys[i] == "validate_ua")
					{
						validate_ua = boost::lexical_cast<bool>(found2->value);
					}
				}
			}

			found1 = std::find_if(cfg_.begin(), cfg_.end(), boost::bind(&cfgpair::compare_first, _1, server_id));
			{
				if (found1 == cfg_.end())
				{
					log->fatal(__FILE__, __LINE__, "config::server_id not found");
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
					found2 = std::find_if(found1->second.begin(), found1->second.end(), boost::bind(&name_value::compare_name, _1, keys[i]));
					if (found2 == found1->second.end())
					{
						log->fatal(__FILE__, __LINE__, "config::element not found");
						return false;
					}

					if (keys[i] == "address")
					{
						address = found2->value;
					}
					else if (keys[i] == "port")
					{
						port = found2->value;
					}
					else if (keys[i] == "doc_root")
					{
						doc_root = found2->value;

						if (doc_root[doc_root.size() - 1] != '/')
						{
							doc_root += "/";
						}
					}
					else if (keys[i] == "num_threads")
					{
						num_threads = boost::lexical_cast<std::size_t>(found2->value);
					}
				}
			}

			std::string msg_file("../msg/");
			{
				msg_file.append(msg_locale);
				msg_file.append(".json");
			}

			if (!boost::filesystem::exists(msg_file))
			{
				log->fatal(__FILE__, __LINE__, "config::msg_file not found");
				return false;
			}
			
			read_json(msg_file, pt);

			int status_code = 0;
			std::vector<statuspair>::iterator status_found;
			BOOST_FOREACH(boost::property_tree::ptree::value_type const& item_, pt.get_child(""))
			{
				status_code = boost::lexical_cast<int>(item_.first);
				status_found = std::find_if(status_.begin(), status_.end(), boost::bind(&statuspair::compare_first, _1, status_code));

				if (status_found != status_.end())
				{
					log->fatal(__FILE__, __LINE__, "config - duplicated status_code:" + boost::lexical_cast<std::string>(status_code));
					return false;
				}
				status_.push_back(statuspair(status_code, item_.second.get_value<std::string>()));
			}

			int status_count = 0;
			for (status_code = static_cast<int>(status_okay); status_code < static_cast<int>(status_end); ++status_code)
			{
				status_found = std::find_if(status_.begin(), status_.end(), boost::bind(&statuspair::compare_first, _1, status_code));
				if (status_found != status_.end())
				{
					++status_count;
				}
			}

			if (status_count != (status_.size() - 1))
			{
				log->fatal(__FILE__, __LINE__, "config::status_count not match");
				return false;
			}

			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

	std::vector<name_value>& config::get(const std::string& item)
	{
		std::vector<cfgpair>::iterator found;
		found = std::find_if(cfg_.begin(), cfg_.end(), boost::bind(&cfgpair::compare_first, _1, item));

		if (found == cfg_.end())
		{
			throw std::runtime_error("config::get failed");
		}

		return found->second;
	}

	const std::string& config::msg(app_status_type status_code)
	{
		int status = static_cast<int>(status_code);

		std::vector<statuspair>::iterator found;
		found = std::find_if(status_.begin(), status_.end(), boost::bind(&statuspair::compare_first, _1, status));

		if (found == status_.end())
		{
			status_.push_back(statuspair(status, "error"));
			found = status_.end() - 1;
		}

		return found->second;
	}

} // namespace waspp
