/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef uri_conn_hpp
#define uri_conn_hpp

#include <unordered_map>

#include <boost/asio.hpp>

namespace waspp
{
	
	enum uri_state
	{
		state_type = 1,
		state_slash_1 = 2,
		state_slash_2 = 3,
		state_host = 4,
		state_port = 5,
		state_path = 6,
	};

	enum class uri_type
	{
		tcp = 1,
		http = 2,

		ssl = 3,
		https = 4,
	};

	class uri_conn
	{
	public:
		uri_conn(const std::string& uri);
		~uri_conn();

		void set_http_headers(const std::unordered_map<std::string, std::string>& req_headers_) { req_headers = req_headers_; }
		bool query(const std::string& data = "", bool force_http_post = false);

		const std::string& res_headers() { return res_headers_; }
		const std::string& res_content() { return res_content_; }

	private:
		bool is_char(int c);
		bool is_ctl(int c);
		bool is_digit(int c);
		bool parse();

		void prepare_http_query(std::ostream& req_stream, const std::string& method, const std::string& data);

		template<typename T>
		bool execute_tcp_query(T& socket_, std::ostream& req_stream, const std::string& data)
		{
			try
			{
				if (!data.empty())
				{
					req_stream << data;

					// Send the request.
					boost::asio::write(socket_, req_buf);
				}

				boost::system::error_code error;
				boost::asio::read(socket_, res_buf, boost::asio::transfer_at_least(1), error);

				if (error)
				{
					return false;
				}

				std::ostringstream oss;
				oss << &res_buf;

				res_content_ = oss.str();
				return true;
			}
			catch (...)
			{

			}

			return false;
		}

		template<typename T>
		bool execute_http_query(T& socket_)
		{
			try
			{
				// Send the request.
				boost::asio::write(socket_, req_buf);

				// Read the response status line. The response streambuf will automatically
				// grow to accommodate the entire line. The growth may be limited by passing
				// a maximum size to the streambuf constructor.
				boost::asio::read_until(socket_, res_buf, "\r\n");

				// Check that response is OK.
				std::istream res_stream(&res_buf);
				if (!is_200(res_stream))
				{
					return false;
				}

				// Read the response headers, which are terminated by a blank line.
				boost::asio::read_until(socket_, res_buf, "\r\n\r\n");

				// Process the response headers.
				while (std::getline(res_stream, res_headers_) && res_headers_ != "\r")
				{
				}

				// Write whatever content we already have to output.
				std::ostringstream oss;
				if (res_buf.size() > 0)
				{
					oss << &res_buf;
				}

				// Read until EOF, writing data to output as we go.
				boost::system::error_code error;
				while (boost::asio::read(socket_, res_buf,
					boost::asio::transfer_at_least(1), error))
				{
					oss << &res_buf;
				}

				if (error != boost::asio::error::eof)
				{
					throw boost::system::system_error(error);
				}

				res_content_ = oss.str();
				return true;
			}
			catch (...)
			{

			}

			return false;
		}

		bool is_200(std::istream& res_stream);

		boost::asio::io_service io_service_;

		uri_state uri_state_;
		uri_type uri_type_;
		std::string uri, type, host, port, path;

		std::unordered_map<std::string, std::string> req_headers;
		boost::asio::streambuf req_buf;

		boost::asio::streambuf res_buf;
		std::string res_headers_;
		std::string res_content_;

	};

} // namespace waspp

#endif // uri_conn_hpp
