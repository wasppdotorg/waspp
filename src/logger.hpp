//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef logger_hpp
#define logger_hpp

#include <thread>
#include <fstream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/asio.hpp>

#include "singleton.hpp"

namespace waspp
{

	enum class log_rotation_type
	{
		every_minute,
		hourly,
		daily
	};

	enum log_level_type
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

		log_level_type log_level() { return log_level_; }
		const std::locale& log_locale() { return log_locale_; }

		void file(const std::string& file);
		bool init(const std::string& log_level, const std::string& log_rotation, int unflushed_limit);

		/// Log a message.
		void write(const boost::posix_time::ptime& ptime_, const std::string& message);

	private:
		void rotate(const std::tm& tm_);

		/// Private io_service used for performing logging operations.
		boost::asio::io_service io_service_;

		/// Work for the private io_service to perform. If we do not give the
		/// io_service some work to do then the io_service::run() function will exit
		/// immediately.
		std::unique_ptr<boost::asio::io_service::work> work_;

		/// Thread used for running the work io_service's run loop.
		std::unique_ptr<std::thread> thread_;

		/// The file to which log messages will be written.
		std::ofstream ofstream_;

		std::string file_;
		std::tm file_created_;

		log_level_type log_level_;
		log_rotation_type log_rotation_;

		std::locale log_locale_;

		int unflushed_count_;
		int unflushed_limit_;

	};

	class log
	{
	public:
		log(const log&) = delete;
		log& operator=(const log&) = delete;

		log(log_level_type log_level) : logger_(*logger::instance()), is_logging(false), ptime_(boost::posix_time::microsec_clock::local_time())
		{
			if (log_level < logger_.log_level())
			{
				return;
			}

			is_logging = true;

			oss.imbue(logger_.log_locale());
			oss << ptime_;

			switch (log_level)
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
				logger_.write(ptime_, oss.str());
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
		logger& logger_;
		bool is_logging;
		boost::posix_time::ptime ptime_;

		std::ostringstream oss;

	};

} // namespace waspp

#endif // logger_hpp
