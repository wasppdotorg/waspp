//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "logger.hpp"

namespace waspp
{

	/// Constructor creates a thread to run a private io_service.
	logger::logger()
		: log_service_(),
		log_work_(new boost::asio::io_service::work(log_service_)),
		log_thread_(new boost::thread(
		boost::bind(&boost::asio::io_service::run, &log_service_)))
	{
		std::time_t time_ = std::time(0);
		file_created_ = *std::localtime(&time_);
	}

	/// Destructor shuts down the private io_service.
	logger::~logger()
	{
		/// Indicate that we have finished with the private io_service. Its
		/// io_service::run() function will exit once all other work has completed.
		log_work_.reset();
		if (log_thread_)
		{
			log_thread_->join();
		}
	}

	void logger::file(const char* file)
	{
		// Pass the work of opening the file to the background thread.
		log_service_.post(boost::bind(&logger::file_impl, this, std::string(file)));

		log_level default_level = log_debug;
		rotation_type default_rotation = rotate_hourly;
		log_service_.post(boost::bind(&logger::init_impl, this, default_level, default_rotation));
	}

	/// Set the output file for the logger. The current implementation sets the
	/// output file for all logger instances, and so the impl parameter is not
	/// actually needed. It is retained here to illustrate how service functions
	/// are typically defined.
	bool logger::init(const std::string& level, const std::string& rotation)
	{
		log_level cfg_level = log_debug;
		rotation_type cfg_rotation = rotate_hourly;

		if (level == "debug")
		{
			cfg_level = log_debug;
		}
		else if (level == "info")
		{
			cfg_level = log_info;
		}
		else if (level == "warn")
		{
			cfg_level = log_warn;
		}
		else if (level == "error")
		{
			cfg_level = log_error;
		}
		else
		{
			return false;
		}

		if (rotation == "minutely")
		{
			cfg_rotation = rotate_minutely;
		}
		else if (rotation == "hourly")
		{
			cfg_rotation = rotate_hourly;
		}
		else if (rotation == "daily")
		{
			cfg_rotation = rotate_daily;
		}
		else if (rotation == "monthly")
		{
			cfg_rotation = rotate_monthly;
		}
		else
		{
			return false;
		}

		log_service_.post(boost::bind(&logger::init_impl, this, cfg_level, cfg_rotation));
		return true;
	}

	/// Log a message.
	void logger::debug(const std::string& message)
	{
		if (level_ > log_debug)
		{
			return;
		}

		log("DEBUG,", message);
	}

	void logger::info(const std::string& message)
	{
		if (level_ > log_info)
		{
			return;
		}

		log("INFO,", message);
	}

	void logger::warn(const std::string& message)
	{
		if (level_ > log_warn)
		{
			return;
		}

		log("WARN,", message);
	}

	void logger::error(const std::string& message)
	{
		if (level_ > log_error)
		{
			return;
		}

		log("ERROR,", message);
	}

	void logger::fatal(const std::string& message)
	{
		if (level_ > log_fatal)
		{
			return;
		}

		log("FATAL,", message);
	}

	void logger::log(const std::string& log_type, const std::string& message)
	{
		std::time_t time_ = std::time(0);
		std::tm time = *std::localtime(&time_);

		// rotate log file if necessary
		log_rotate(time);

		// datetime for log message
		char datetime[32] = {0};
		std::strftime(datetime, sizeof(datetime), "%Y-%m-%d %H:%M:%S,", &time);

		std::string line(datetime);
		{
			line.append(log_type);
			line.append(message);
		}

		// Pass the work of opening the file to the background thread.
		log_service_.post(boost::bind(&logger::log_impl, this, line));
	}
	
	void logger::log_rotate(const std::tm& time)
	{
		char datetime[32] = {0};

		if (rotation_ == rotate_minutely && time.tm_min != file_created_.tm_min)
		{
			std::strftime(datetime, sizeof(datetime), "%Y-%m-%d_%H%M_", &file_created_);
		}
		else if (rotation_ == rotate_hourly && time.tm_hour != file_created_.tm_hour)
		{
			std::strftime(datetime, sizeof(datetime), "%Y-%m-%d_%H_", &file_created_);
		}
		else if (rotation_ == rotate_daily && time.tm_mday != file_created_.tm_mday)
		{
			std::strftime(datetime, sizeof(datetime), "%Y-%m-%d_", &file_created_);
		}
		else if (rotation_ == rotate_monthly && time.tm_mon != file_created_.tm_mon)
		{
			std::strftime(datetime, sizeof(datetime), "%Y-%m_", &file_created_);
		}
		else
		{
			return;
		}

		std::string file_to(datetime);
		file_to += file_;

		log_service_.post(boost::bind(&logger::log_rotate_impl, this, file_to));
	}

	/// Helper function used to open the output file from within the private
	/// io_service's thread.
	void logger::file_impl(const std::string& file)
	{
		file_ = file;

		std::time_t time_ = std::time(0);
		file_created_ = *std::localtime(&time_);

		ofstream_.close();
		ofstream_.clear();
		ofstream_.open(file_.c_str(), std::fstream::app);
	}

	void logger::init_impl(log_level level, rotation_type rotation)
	{
		level_ = level;
		rotation_ = rotation;
	}

	/// Helper function used to log a message from within the private io_service's
	/// thread.
	void logger::log_impl(const std::string& line)
	{
		ofstream_ << line << std::endl;
	}

	void logger::log_rotate_impl(const std::string& file_to)
	{
		std::ifstream is(file_to.c_str(), std::ios::in | std::ios::binary);
		if (is)
		{
			return;
		}

		ofstream_.close();
		boost::filesystem::rename(file_, file_to);

		file_impl(file_);
	}

} // namespace waspp
