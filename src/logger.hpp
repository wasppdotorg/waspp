//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_LOGGER_HPP
#define WASPP_LOGGER_HPP

#include <ctime>

#include <fstream>
#include <string>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "utility.hpp"

namespace waspp
{

	enum rotation_type
	{
		rotate_minutely,
		rotate_hourly,
		rotate_daily
	};

	enum log_level
	{
		debug = 1,
		info = 2,
		warn = 3,
		error = 4,
		fatal = 5
	};

	class logger
		: public singleton<logger>
	{
	public:
		logger();
		~logger();

		log_level level() { return level_; }

		void file(const std::string& file);
		bool init(const std::string& level, const std::string& rotation);

		/// Log a message.
		void write(const std::string& message);

	private:

		void rotate(const std::tm& tm_);

		void file_impl(const std::string& file);
		void init_impl(log_level level, rotation_type rotation);
		void write_impl(const std::string& line);
		void rotate_impl(const std::string& file_to);

		/// Private io_service used for performing logging operations.
		boost::asio::io_service log_service_;

		/// Work for the private io_service to perform. If we do not give the
		/// io_service some work to do then the io_service::run() function will exit
		/// immediately.
		boost::asio::io_service::work log_work_;

		/// Thread used for running the work io_service's run loop.
		boost::scoped_ptr<boost::thread> log_thread_;

		/// The file to which log messages will be written.
		std::ofstream ofstream_;

		std::string file_;
		std::tm file_created_;

		log_level level_;
		rotation_type rotation_;

	};

	class log
	{
	public:
		log(log_level level) : is_logging(false)
		{
			logger_ = logger::instance();

			if (level < logger_->level())
			{
				return;
			}

			is_logging = true;

			// datetime for log message
			oss.imbue(std::locale(std::cout.getloc(), new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S,%f,")));
			oss << boost::posix_time::microsec_clock::local_time();

			switch (level)
			{
			case debug:
				oss << "DEBUG,";
				break;
			case info:
				oss << "INFO,";
				break;
			case warn:
				oss << "WARN,";
				break;
			case error:
				oss << "ERROR,";
				break;
			case fatal:
				oss << "FATAL,";
				break;
			default:
				oss << "UNKNOWN,";
				break;
			}
		}

		~log()
		{
			if (is_logging)
			{
				logger_->write(oss.str());
			}
		}

		template<typename T>
		std::ostringstream& operator<<(T v)
		{
			if (is_logging)
			{
				oss << v;
			}

			return oss;
		}

	private:
		log() {}

		logger* logger_;
		bool is_logging;

		std::ostringstream oss;
	};

} // namespace waspp

#endif // WASPP_LOGGER_HPP
