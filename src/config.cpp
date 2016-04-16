/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

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
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>

#include "config.hpp"
#include "logger.hpp"

namespace waspp
{

	config::config() : unflushed_limit_(0), expiry_sec_(0), update_sec_(0), validate_ip_(false), validate_ua_(false), num_threads_(0), compress_(false), ssl_(false)
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

			boost::property_tree::ptree ptree_;
			boost::property_tree::json_parser::read_json(file, ptree_);

			//boost::property_tree::ptree::const_iterator section;
			//boost::property_tree::ptree::const_iterator item;
			for (auto& section : ptree_)
			{
				boost::unordered_map<std::string, std::string> c;
				for (auto& item : section.second)
				{
					c.insert(std::make_pair(item.first, item.second.get_value<std::string>()));
				}

				cfg_.insert(std::make_pair(section.first, c));
			}

			std::vector<std::string> keys;
			//boost::unordered_map< std::string, boost::unordered_map<std::string, std::string> >::iterator found_section;
			//boost::unordered_map<std::string, std::string>::iterator found_item;

			auto found_section = cfg_.find("log");
			{
				if (found_section == cfg_.end())
				{
					log(fatal) << "config::log not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("level");
					keys.push_back("rotation");
					keys.push_back("unflushed_limit");
				}

				for (auto& key : keys)
				{
					auto found_item = found_section->second.find(key);
					if (found_item == found_section->second.end())
					{
						log(fatal) << "config::element not found," << __FILE__ << ":" << __LINE__;
						return false;
					}

					if (key == "level")
					{
						log_level_ = found_item->second;
					}
					else if (key == "rotation")
					{
						log_rotation_ = found_item->second;
					}
					else if (key == "unflushed_limit")
					{
						unflushed_limit_ = boost::lexical_cast<int>(found_item->second);
					}
				}
			}

			found_section = cfg_.find("error");
			{
				if (found_section == cfg_.end())
				{
					log(fatal) << "config::error not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				keys.resize(0);
				{
					keys.push_back("locale");
				}

				for (auto& key : keys)
				{
					auto found_item = found_section->second.find(key);
					if (found_item == found_section->second.end())
					{
						log(fatal) << "config::element not found," << __FILE__ << ":" << __LINE__;
						return false;
					}

					if (key == "locale")
					{
						locale = found_item->second;
					}
				}
			}

			found_section = cfg_.find("access_granted");
			{
				if (found_section == cfg_.end())
				{
					log(fatal) << "config::access_granted not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				for (auto& found_item : found_section->second)
				{
					access_granted_.push_back(found_item.second);
				}
			}

			found_section = cfg_.find("access_denied");
			{
				if (found_section == cfg_.end())
				{
					log(fatal) << "config::denied not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				for (auto& found_item : found_section->second)
				{
					access_denied_.push_back(found_item.second);
				}
			}

			found_section = cfg_.find("session");
			{
				if (found_section == cfg_.end())
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
					keys.push_back("validate_ip");
					keys.push_back("validate_ua");
				}

				for (auto& key : keys)
				{
					auto found_item = found_section->second.find(key);
					if (found_item == found_section->second.end())
					{
						log(fatal) << "config::element not found," << __FILE__ << ":" << __LINE__;
						return false;
					}

					if (key == "encrypt_key")
					{
						encrypt_key_ = found_item->second;
					}
					else if (key == "sess_cookie")
					{
						sess_cookie_ = found_item->second;
					}
					else if (key == "expiry_sec")
					{
						expiry_sec_ = boost::lexical_cast<double>(found_item->second);
					}
					else if (key == "update_sec")
					{
						update_sec_ = boost::lexical_cast<double>(found_item->second);
					}
					else if (key == "validate_ip")
					{
						validate_ip_ = boost::lexical_cast<bool>(found_item->second);
					}
					else if (key == "validate_ua")
					{
						validate_ua_ = boost::lexical_cast<bool>(found_item->second);
					}
				}
			}

			found_section = cfg_.find(server_id);
			{
				if (found_section == cfg_.end())
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

				for (auto& key : keys)
				{
					auto found_item = found_section->second.find(key);
					if (found_item == found_section->second.end())
					{
						log(fatal) << "config::element not found," << __FILE__ << ":" << __LINE__;
						return false;
					}

					if (key == "address")
					{
						address_ = found_item->second;
					}
					else if (key == "port")
					{
						port_ = found_item->second;
					}
					else if (key == "doc_root")
					{
						doc_root_ = found_item->second;

						if (doc_root_[doc_root_.size() - 1] != '/')
						{
							doc_root_ += "/";
						}
					}
					else if (key == "num_threads")
					{
						num_threads_ = boost::lexical_cast<std::size_t>(found_item->second);
					}
					else if (key == "compress")
					{
						compress_ = boost::lexical_cast<bool>(found_item->second);
					}
					else if (key == "ssl")
					{
						ssl_ = boost::lexical_cast<bool>(found_item->second);
					}
					else if (key == "ssl_crt")
					{
						ssl_crt_ = found_item->second;
					}
					else if (key == "ssl_key")
					{
						ssl_key_ = found_item->second;
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

			boost::property_tree::json_parser::read_json(err_file, ptree_);

			//int err_code = 0;
			//boost::unordered_map<int, std::string>::iterator err_found;

			for (auto& item : ptree_)
			{
				int err_code = boost::lexical_cast<int>(item.first);
				auto err_found = err_.find(err_code);

				if (err_found != err_.end())
				{
					log(fatal) << "config - duplicated err_code:" << err_code << "," << __FILE__ << ":" << __LINE__;
					return false;
				}
				err_.insert(std::make_pair(err_code, item.second.get_value<std::string>()));
			}

			unsigned int err_count = 0;
			for (int err_code = err_none; err_code < err_end; ++err_code)
			{
				auto err_found = err_.find(err_code);
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
		//boost::unordered_map< std::string, boost::unordered_map<std::string, std::string> >::iterator found;
		auto found = cfg_.find(item);

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
