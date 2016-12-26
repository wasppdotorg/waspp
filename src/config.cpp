/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/filesystem.hpp>

// when using both boost asio and boost ptree on win32
// put these lines below before json_parser.hpp
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#endif

#include <boost/property_tree/json_parser.hpp>

#include "config.hpp"
#include "logger.hpp"

namespace waspp
{

	config::config() : log_unflushed_limit_(0), expiry_sec_(0), update_sec_(0), validate_ip_(false), validate_ua_(false), num_threads_(0), compress_(false), ssl_(false)
	{
		
	}

	config::~config()
	{
		
	}

	bool config::init(const std::string& addr, const std::string& file)
	{
		try
		{
			address_ = addr;

			if (!boost::filesystem::exists(file))
			{
				log(fatal) << "config::file not found," << __FILE__ << ":" << __LINE__;
				return false;
			}

			boost::property_tree::ptree ptree_;
			boost::property_tree::json_parser::read_json(file, ptree_);

			for (auto& section : ptree_)
			{
				std::unordered_map<std::string, std::string> c;
				for (auto& item : section.second)
				{
					c.insert(std::make_pair(item.first, item.second.get_value<std::string>()));
				}

				cfg_.insert(std::make_pair(section.first, c));
			}

			std::vector<std::string> keys;
			auto found_section = cfg_.find("log");
			//
				if (found_section == cfg_.end())
				{
					log(fatal) << "config::log not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				keys = { "level", "rotation", "unflushed_limit" };
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
						log_unflushed_limit_ = strtol(found_item->second.c_str(), nullptr, 0);
					}
				}
			//

			found_section = cfg_.find("error");
			//
				if (found_section == cfg_.end())
				{
					log(fatal) << "config::error not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				keys = { "locale" };
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
						locale_ = found_item->second;
					}
				}
			//

			found_section = cfg_.find("access_granted");
			//
				if (found_section == cfg_.end())
				{
					log(fatal) << "config::access_granted not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				for (auto& found_item : found_section->second)
				{
					access_granted_.push_back(found_item.second);
				}
			//

			found_section = cfg_.find("access_denied");
			//
				if (found_section == cfg_.end())
				{
					log(fatal) << "config::denied not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				for (auto& found_item : found_section->second)
				{
					access_denied_.push_back(found_item.second);
				}
			//

			found_section = cfg_.find("session");
			//
				if (found_section == cfg_.end())
				{
					log(fatal) << "config::session not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				keys = { "encrypt_key", "sess_cookie", "expiry_sec", "update_sec", "validate_ip", "validate_ua" };
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
						expiry_sec_ = strtod(found_item->second.c_str(), nullptr);
					}
					else if (key == "update_sec")
					{
						update_sec_ = strtod(found_item->second.c_str(), nullptr);
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
			//

			found_section = cfg_.find("server");
			//
				if (found_section == cfg_.end())
				{
					log(fatal) << "config::server not found," << __FILE__ << ":" << __LINE__;
					return false;
				}

				keys = { "port", "doc_root", "num_threads", "compress", "ssl", "ssl_crt", "ssl_key" };
				for (auto& key : keys)
				{
					auto found_item = found_section->second.find(key);
					if (found_item == found_section->second.end())
					{
						log(fatal) << "config::element not found," << __FILE__ << ":" << __LINE__;
						return false;
					}

					if (key == "port")
					{
						port_ = found_item->second;
					}
					else if (key == "doc_root")
					{
						doc_root_ = found_item->second;

						if (doc_root_[doc_root_.size() - 1] != '/')
						{
							doc_root_ += '/';
						}
					}
					else if (key == "num_threads")
					{
						num_threads_ = strtoull(found_item->second.c_str(), nullptr, 0);
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
			//

			std::string err_file("../err/");
			//
				err_file.append(locale_);
				err_file.append(".json");
			//

			if (!boost::filesystem::exists(err_file))
			{
				log(fatal) << "config::err_file not found," << __FILE__ << ":" << __LINE__;
				return false;
			}
			boost::property_tree::json_parser::read_json(err_file, ptree_);

			int err_code = 0;
			for (auto& item : ptree_)
			{
				err_code = strtol(item.first.c_str(), nullptr, 0);
				if (err_.find(err_code) != err_.end())
				{
					log(fatal) << "config - duplicated err_code:" << err_code << "," << __FILE__ << ":" << __LINE__;
					return false;
				}

				err_.insert(std::make_pair(err_code, item.second.get_value<std::string>()));
			}

			return true;
		}
		catch (...)
		{
			throw;
		}

		return false;
	}

	std::unordered_map<std::string, std::string>& config::get(const std::string& item)
	{
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
