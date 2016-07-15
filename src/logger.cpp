//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/filesystem.hpp>

#include "logger.hpp"

namespace waspp
{

	/// Constructor creates a thread to run a private io_service.
	logger::logger()
		: io_service_(),
		work_(new boost::asio::io_service::work(io_service_)),
		thread_(new std::thread([this](){ io_service_.run(); })),
		log_level_(debug),
		log_rotation_(log_rotation_type::every_minute),
		log_locale_(std::cout.getloc(), new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S,%f,")),
		unflushed_count_(0),
		unflushed_limit_(255)
	{
		auto time_ = std::time(nullptr);
		file_created_ = *std::localtime(&time_);
	}

	/// Destructor shuts down the private io_service.
	logger::~logger()
	{
		/// Indicate that we have finished with the private io_service. Its
		/// io_service::run() function will exit once all other work has completed.
		work_.reset();
		if (thread_)
		{
			thread_->join();
		}

		if (ofstream_.is_open())
		{
			ofstream_.flush();
			ofstream_.close();
		}
	}

	void logger::file(const std::string& file)
	{
		// Pass the work of opening the file to the background thread.
		io_service_.post([this, file]()
		{
			file_ = file;

			auto time_ = std::time(nullptr);
			file_created_ = *std::localtime(&time_);

			ofstream_.close();
			ofstream_.clear();
			ofstream_.open(file_.c_str(), std::fstream::app);
		});
	}

	/// Set the output file for the logger. The current implementation sets the
	/// output file for all logger instances, and so the impl parameter is not
	/// actually needed. It is retained here to illustrate how service functions
	/// are typically defined.
	bool logger::init(const std::string& log_level, const std::string& log_rotation, int unflushed_limit)
	{
		log_level_type cfg_log_level = debug;

		if (log_level == "debug")
		{
			cfg_log_level = debug;
		}
		else if (log_level == "info")
		{
			cfg_log_level = info;
		}
		else if (log_level == "warn")
		{
			cfg_log_level = warn;
		}
		else if (log_level == "error")
		{
			cfg_log_level = error;
		}
		else
		{
			return false;
		}

		log_rotation_type cfg_log_rotation = log_rotation_type::every_minute;

		if (log_rotation == "every_minute")
		{
			cfg_log_rotation = log_rotation_type::every_minute;
		}
		else if (log_rotation == "hourly")
		{
			cfg_log_rotation = log_rotation_type::hourly;
		}
		else if (log_rotation == "daily")
		{
			cfg_log_rotation = log_rotation_type::daily;
		}
		else
		{
			return false;
		}

		unflushed_limit_ = unflushed_limit;

		io_service_.post([this, cfg_log_level, cfg_log_rotation]()
		{
			log_level_ = cfg_log_level;
			log_rotation_ = cfg_log_rotation;
		});

		return true;
	}

	/// Log a message.
	void logger::write(const boost::posix_time::ptime& ptime_, const std::string& message)
	{
		rotate(boost::posix_time::to_tm(ptime_));

		// Pass the work of opening the file to the background thread.
		io_service_.post([this, message]()
		{
			ofstream_ << message << std::endl;

			if (++unflushed_count_ > unflushed_limit_)
			{
				ofstream_.flush();
				unflushed_count_ = 0;
			}
		});
	}

	void logger::rotate(const std::tm& tm_)
	{
		char datetime[32] = { 0 };

		if (log_rotation_ == log_rotation_type::every_minute && tm_.tm_min != file_created_.tm_min)
		{
			std::strftime(datetime, sizeof(datetime), ".%Y-%m-%d_%H%M", &file_created_);
		}
		else if (log_rotation_ == log_rotation_type::hourly && tm_.tm_hour != file_created_.tm_hour)
		{
			std::strftime(datetime, sizeof(datetime), ".%Y-%m-%d_%H", &file_created_);
		}
		else if (log_rotation_ == log_rotation_type::daily && tm_.tm_mday != file_created_.tm_mday)
		{
			std::strftime(datetime, sizeof(datetime), ".%Y-%m-%d", &file_created_);
		}
		else
		{
			return;
		}

		std::string file_to(file_);
		file_to.append(datetime);

		io_service_.post([this, file_to]()
		{
			std::ifstream is(file_to.c_str(), std::ios::in | std::ios::binary);
			if (is)
			{
				return;
			}

			ofstream_.close();
			boost::filesystem::rename(file_, file_to);

			auto time_ = std::time(nullptr);
			file_created_ = *std::localtime(&time_);

			ofstream_.clear();
			ofstream_.open(file_.c_str(), std::fstream::app);
		});
	}

} // namespace waspp
