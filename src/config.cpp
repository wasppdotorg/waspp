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
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>

#include "config.hpp"
#include "logger.hpp"

namespace waspp
{

	config::config() : expiry_sec_(0), update_sec_(0), validate_ep_(false), validate_ua_(false), num_threads_(0), compress_(false), ssl_(false)
	{
	}

	config::~config()
	{
	}

	bool config::init(const std::string& file, const std::string& server_id)
	{
		try
		{
			if (!boost::filesystem::exists(file))
			{
				log(fatal) << "config::file not found," << __FILE__ << ":" << __LINE__;
				return false;
			}

			boost::property_tree::ptree pt;
			boost::property_tree::json_parser::read_json(file, pt);

			BOOST_FOREACH(boost::property_tree::ptree::value_type const& item_, pt.get_child(""))
			{
				boost::unordered_map<std::string, std::string> i;
				BOOST_FOREACH(boost::property_tree::ptree::value_type const& pair_, item_.second)
				{
					i.insert(std::make_pair(pair_.first, pair_.second.get_value<std::string>()));
				}

				cfg_.insert(std::make_pair(item_.first, i));
			}

			std::vector<std::string> keys;
			boost::unordered_map< std::string, boost::unordered_map<std::string, std::string> >::iterator found_i;
			boost::unordered_map<std::string, std::string>::iterator found_p;

			found_i = cfg_.find("log");
			{
				if (found_i == cfg_.end())
				{
					log(fatal) << "config::log not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("level");
					keys.push_back("rotation");
				}

				for (std::size_t i = 0; i < keys.size(); ++i)
				{
					found_p = found_i->second.find(keys[i]);
					if (found_p == found_i->second.end())
					{
						log(fatal) << "config::element not found," << __FILE__ << ":" << __LINE__;
						return false;
					}

					if (keys[i] == "level")
					{
						log_level_ = found_p->second;
					}
					else if (keys[i] == "rotation")
					{
						log_rotation_ = found_p->second;
					}
				}
			}

			found_i = cfg_.find("error");
			{
				if (found_i == cfg_.end())
				{
					log(fatal) << "config::error not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("locale");
				}

				for (std::size_t i = 0; i < keys.size(); ++i)
				{
					found_p = found_i->second.find(keys[i]);
					if (found_p == found_i->second.end())
					{
						log(fatal) << "config::element not found," << __FILE__ << ":" << __LINE__;
						return false;
					}

					if (keys[i] == "locale")
					{
						locale = found_p->second;
					}
				}
			}

			found_i = cfg_.find("access_granted");
			{
				if (found_i == cfg_.end())
				{
					log(fatal) << "config::access_granted not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				for (found_p = found_i->second.begin(); found_p != found_i->second.end(); ++found_p)
				{
					access_granted_.push_back(found_p->second);
				}
			}

			found_i = cfg_.find("access_denied");
			{
				if (found_i == cfg_.end())
				{
					log(fatal) << "config::denied not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				for (found_p = found_i->second.begin(); found_p != found_i->second.end(); ++found_p)
				{
					access_denied_.push_back(found_p->second);
				}
			}

			found_i = cfg_.find("session");
			{
				if (found_i == cfg_.end())
				{
					log(fatal) << "config::session not found," << __FILE__ << ":" << __LINE__;
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
					found_p = found_i->second.find(keys[i]);
					if (found_p == found_i->second.end())
					{
						log(fatal) << "config::element not found," << __FILE__ << ":" << __LINE__;
						return false;
					}

					if (keys[i] == "encrypt_key")
					{
						encrypt_key_ = found_p->second;
					}
					else if (keys[i] == "sess_cookie")
					{
						sess_cookie_ = found_p->second;
					}
					else if (keys[i] == "expiry_sec")
					{
						expiry_sec_ = boost::lexical_cast<double>(found_p->second);
					}
					else if (keys[i] == "update_sec")
					{
						update_sec_ = boost::lexical_cast<double>(found_p->second);
					}
					else if (keys[i] == "validate_ep")
					{
						validate_ep_ = boost::lexical_cast<bool>(found_p->second);
					}
					else if (keys[i] == "validate_ua")
					{
						validate_ua_ = boost::lexical_cast<bool>(found_p->second);
					}
				}
			}

			found_i = cfg_.find(server_id);
			{
				if (found_i == cfg_.end())
				{
					log(fatal) << "config::server_id not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("address");
					keys.push_back("port");
					keys.push_back("doc_root");
					keys.push_back("num_threads");
					keys.push_back("compress");

					keys.push_back("ssl");
					keys.push_back("ssl_crt");
					keys.push_back("ssl_key");
				}

				for (std::size_t i = 0; i < keys.size(); ++i)
				{
					found_p = found_i->second.find(keys[i]);
					if (found_p == found_i->second.end())
					{
						log(fatal) << "config::element not found," << __FILE__ << ":" << __LINE__;
						return false;
					}

					if (keys[i] == "address")
					{
						address_ = found_p->second;
					}
					else if (keys[i] == "port")
					{
						port_ = found_p->second;
					}
					else if (keys[i] == "doc_root")
					{
						doc_root_ = found_p->second;

						if (doc_root_[doc_root_.size() - 1] != '/')
						{
							doc_root_ += "/";
						}
					}
					else if (keys[i] == "num_threads")
					{
						num_threads_ = boost::lexical_cast<std::size_t>(found_p->second);
					}
					else if (keys[i] == "compress")
					{
						compress_ = boost::lexical_cast<bool>(found_p->second);
					}
					else if (keys[i] == "ssl")
					{
						ssl_ = boost::lexical_cast<bool>(found_p->second);
					}
					else if (keys[i] == "ssl_crt")
					{
						ssl_crt_ = found_p->second;
					}
					else if (keys[i] == "ssl_key")
					{
						ssl_key_ = found_p->second;
					}
				}
			}

			std::string err_file("../err/");
			{
				err_file.append(locale);
				err_file.append(".json");
			}

			if (!boost::filesystem::exists(err_file))
			{
				log(fatal) << "config::err_file not found," << __FILE__ << ":" << __LINE__;
				return false;
			}

			boost::property_tree::json_parser::read_json(err_file, pt);

			int err_code = 0;
			boost::unordered_map<int, std::string>::iterator err_found;
			BOOST_FOREACH(boost::property_tree::ptree::value_type const& item_, pt.get_child(""))
			{
				err_code = boost::lexical_cast<int>(item_.first);
				err_found = err_.find(err_code);

				if (err_found != err_.end())
				{
					log(fatal) << "config - duplicated err_code:" << err_code << "," << __FILE__ << ":" << __LINE__;
					return false;
				}
				err_.insert(std::make_pair(err_code, item_.second.get_value<std::string>()));
			}

			unsigned int err_count = 0;
			for (err_code = err_none; err_code < err_end; ++err_code)
			{
				err_found = err_.find(err_code);
				if (err_found != err_.end())
				{
					++err_count;
				}
			}

			if (err_count != (err_.size() - 1))
			{
				log(fatal) << "config::err_count not match," << __FILE__ << ":" << __LINE__;
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

	boost::unordered_map<std::string, std::string>& config::get(const std::string& item)
	{
		boost::unordered_map< std::string, boost::unordered_map<std::string, std::string> >::iterator found;
		found = cfg_.find(item);

		if (found == cfg_.end())
		{
			throw std::runtime_error("config::get failed");
		}

		return found->second;
	}

	const std::string& config::err_msg(error_type err_code)
	{
		return err_[err_code];
	}

} // namespace waspp
