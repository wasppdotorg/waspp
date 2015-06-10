//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>

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

		level_ = debug;
		rotation_ = rotate_minutely;
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

		if (ofstream_.is_open())
		{
			ofstream_.close();
		}
	}

	void logger::file(const std::string& file)
	{
		// Pass the work of opening the file to the background thread.
		log_service_.post(boost::bind(&logger::file_impl, this, std::string(file)));
	}

	/// Set the output file for the logger. The current implementation sets the
	/// output file for all logger instances, and so the impl parameter is not
	/// actually needed. It is retained here to illustrate how service functions
	/// are typically defined.
	bool logger::init(const std::string& level, const std::string& rotation)
	{
		log_level cfg_level = debug;

		if (level == "debug")
		{
			cfg_level = debug;
		}
		else if (level == "info")
		{
			cfg_level = info;
		}
		else if (level == "warn")
		{
			cfg_level = warn;
		}
		else if (level == "error")
		{
			cfg_level = error;
		}
		else
		{
			return false;
		}

		rotation_type cfg_rotation = rotate_minutely;

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
		else
		{
			return false;
		}

		log_service_.post(boost::bind(&logger::init_impl, this, cfg_level, cfg_rotation));
		return true;
	}

	/// Log a message.
	void logger::write(const std::string& message)
	{
		// Pass the work of opening the file to the background thread.
		log_service_.post(boost::bind(&logger::write_impl, this, message));
	}

	void logger::rotate(const std::tm& tm_)
	{
		char datetime[32] = { 0 };

		if (rotation_ == rotate_minutely && tm_.tm_min != file_created_.tm_min)
		{
			std::strftime(datetime, sizeof(datetime), ".%Y-%m-%d_%H%M", &file_created_);
		}
		else if (rotation_ == rotate_hourly && tm_.tm_hour != file_created_.tm_hour)
		{
			std::strftime(datetime, sizeof(datetime), ".%Y-%m-%d_%H", &file_created_);
		}
		else if (rotation_ == rotate_daily && tm_.tm_mday != file_created_.tm_mday)
		{
			std::strftime(datetime, sizeof(datetime), ".%Y-%m-%d", &file_created_);
		}
		else
		{
			return;
		}

		std::string file_to(file_);
		file_to.append(datetime);

		log_service_.post(boost::bind(&logger::rotate_impl, this, file_to));
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
	void logger::write_impl(const std::string& message)
	{
		ofstream_ << message << std::endl;
	}

	void logger::rotate_impl(const std::string& file_to)
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
