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
	{
	public:
		logger();
		~logger();

		log_level_type level() { return level_; }
		const std::locale& loc() { return loc_; }

		void file(const std::string& file);
		bool init();

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

		log_level_type level_;
		log_rotation_type rotation_;

		std::locale loc_;

		int unflushed_count_;
		int unflushed_limit_;

	};

	class log
	{
	public:
		log(log_level_type log_level);
		~log();

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
