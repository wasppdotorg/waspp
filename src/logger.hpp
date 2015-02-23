//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_LOGGER_HPP
#define WASPP_LOGGER_HPP

#include <ctime>

#include <fstream>
#include <sstream>
#include <string>

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>

#include "singleton.hpp"

namespace waspp
{
	enum rotation_type
	{
		rotate_minutely = 0,
		rotate_hourly = 1,
		rotate_daily = 2,
		rotate_monthly = 3
	};

	enum log_level
	{
		log_debug = 1,
		log_info = 2,
		log_warn = 3,
		log_error = 4,
		log_fatal = 5
	};

	class logger
		: public singleton<logger>
	{
	public:
		logger();
		~logger();

		void file(const std::string& file);
		bool init(const std::string& level, const std::string& rotation);

		/// Log a message.
		void debug(const std::string& message);
		void info(const std::string& message);
		void warn(const std::string& message);
		void error(const std::string& message);
		void fatal(const std::string& message);

	private:
		void log(const std::string& log_type, const std::string& message);
		void log_rotate(const std::tm& time);

		void file_impl(const std::string& file);
		void init_impl(log_level level, rotation_type rotation);
		void log_impl(const std::string& line);
		void log_rotate_impl(const std::string& file_to);

		/// Private io_service used for performing logging operations.
		boost::asio::io_service log_service_;

		/// Work for the private io_service to perform. If we do not give the
		/// io_service some work to do then the io_service::run() function will exit
		/// immediately.
		boost::scoped_ptr<boost::asio::io_service::work> log_work_;

		/// Thread used for running the work io_service's run loop.
		boost::scoped_ptr<boost::thread> log_thread_;

		/// The file to which log messages will be written.
		std::ofstream ofstream_;

		std::string file_;
		std::tm file_created_;

		log_level level_;
		rotation_type rotation_;

	};

} // namespace waspp

#endif // WASPP_LOGGER_HPP
