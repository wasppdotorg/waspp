/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <boost/asio/ssl.hpp>
#include <boost/logic/tribool.hpp>

#include "uri_conn.hpp"

namespace waspp
{

	uri_conn::uri_conn(const std::string& uri_)
		: uri_state_(state_type), uri_type_(uri_type::http), uri(uri_), port("http")
	{
		
	}

	uri_conn::~uri_conn()
	{
		
	}

	bool uri_conn::query(const std::string& data, bool force_http_post)
	{
		try
		{
			if (!parse())
			{
				return false;
			}

			// Get a list of endpoints corresponding to the server name.
			boost::asio::ip::tcp::resolver resolver_(io_service_);
			boost::asio::ip::tcp::resolver::query query_(host, port);
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver_.resolve(query_);

			// Form the request. We specify the "Connection: close" header so that the
			// server will close the socket after transmitting the response. This will
			// allow us to treat all data up until the EOF as the content.
			std::ostream req_stream(&req_buf);

			if (uri_type_ == uri_type::tcp || uri_type_ == uri_type::http)
			{
				// Try each endpoint until we successfully establish a connection.
				boost::asio::ip::tcp::socket socket_(io_service_);
				boost::asio::connect(socket_, endpoint_iterator);

				if (uri_type_ == uri_type::tcp)
				{
					return execute_tcp_query(socket_, req_stream, data);
				}
				else if (uri_type_ == uri_type::http)
				{
					if (force_http_post || !data.empty())
					{
						prepare_http_query(req_stream, "POST", data);
					}
					else
					{
						prepare_http_query(req_stream, "GET", data);
					}

					return execute_http_query(socket_);
				}
				else
				{
					return false;
				}

				return true;
			}
			else if (uri_type_ == uri_type::ssl || uri_type_ == uri_type::https)
			{
				boost::asio::ssl::context context_(io_service_, boost::asio::ssl::context::sslv23);
				context_.set_verify_mode(boost::asio::ssl::verify_none);

				// Try each endpoint until we successfully establish a connection.
				boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_(io_service_, context_);

				boost::asio::connect(socket_.lowest_layer(), endpoint_iterator);
				socket_.handshake(boost::asio::ssl::stream_base::client);

				if (uri_type_ == uri_type::ssl)
				{
					return execute_tcp_query(socket_, req_stream, data);
				}
				else if (uri_type_ == uri_type::https)
				{
					if (force_http_post || !data.empty())
					{
						prepare_http_query(req_stream, "POST", data);
					}
					else
					{
						prepare_http_query(req_stream, "GET", data);
					}

					return execute_http_query(socket_);
				}
				else
				{
					return false;
				}

				return true;
			}
		}
		catch (...)
		{

		}

		return false;
	}
	
	bool uri_conn::is_char(int c)
	{
		return c >= 0 && c <= 127;
	}

	bool uri_conn::is_ctl(int c)
	{
		return (c >= 0 && c <= 31) || (c == 127);
	}

	bool uri_conn::is_digit(int c)
	{
		return c >= '0' && c <= '9';
	}
	
	bool uri_conn::parse()
	{
		char input;
		boost::tribool result = boost::indeterminate;

		auto begin = uri.begin();
		auto end = uri.end();

		while (begin != end)
		{
			switch (uri_state_)
			{
			case state_type:
				input = *begin++;

				if (!is_char(input) || is_ctl(input))
				{
					result = false;
					break;
				}
				else if (input == ':')
				{
					uri_state_ = state_slash_1;
					result = boost::indeterminate;
					break;
				}
				else
				{
					type.push_back(input);
					result = boost::indeterminate;
					break;
				}

			case state_slash_1:
				input = *begin++;

				if (input == '/')
				{
					uri_state_ = state_slash_2;
					result = boost::indeterminate;
					break;
				}
				else
				{
					result = false;
					break;
				}
					
			case state_slash_2:
				input = *begin++;
					
				if (input == '/')
				{
					uri_state_ = state_host;
					result = boost::indeterminate;
					break;
				}
				else
				{
					result = false;
					break;
				}
					
			case state_host:
				do
				{
					input = *begin++;

					if (input == ':')
					{
						uri_state_ = state_port;
						result = boost::indeterminate;
						break;
					}
					else if (input == '/')
					{
						uri_state_ = state_path;
						path.push_back(input);
						result = boost::indeterminate;
						break;
					}
					else if (is_ctl(input))
					{
						result = false;
						break;
					}
					else
					{
						host.push_back(input);
					}

				} while (begin != end);
				break;

			case state_port:
				input = *begin++;

				if (is_digit(input))
				{
					port.push_back(input);
					result = boost::indeterminate;
					break;
				}
				else if (input == '/')
				{
					uri_state_ = state_path;
					path.push_back(input);
					result = boost::indeterminate;
					break;
				}
				else
				{
					result = false;
					break;
				}

			case state_path:
				do
				{
					input = *begin++;

					if (is_ctl(input))
					{
						result = false;
						break;
					}
					else
					{
						path.push_back(input);
					}

				} while (begin != end);
				break;

			default:
				result = false;
					break;
			}
		}

		if (type == "tcp")
		{
			if (port.empty())
			{
				return false;
			}
			
			uri_type_ = uri_type::tcp;
		}
		else if (type == "http")
		{
			if (port.empty())
			{
				port = type;
			}
			
			uri_type_ = uri_type::http;
		}
		else if (type == "ssl")
		{
			if (port.empty())
			{
				return false;
			}
			
			uri_type_ = uri_type::ssl;
		}
		else if (type == "https")
		{
			if (port.empty())
			{
				port = type;
			}
			
			uri_type_ = uri_type::https;
		}
		else
		{
			return false;
		}

		if (result == false)
		{
			return false;
		}
			
		return true;
	}

	void uri_conn::prepare_http_query(std::ostream& req_stream, const std::string& method, const std::string& data)
	{
		req_stream << method << " " << path << " HTTP/1.1\r\n";
		req_stream << "Host: " << host << "\r\n";
		req_stream << "Accept: */*\r\n";
		req_stream << "Connection: close\r\n";

		for (auto& req_header : req_headers)
		{
			req_stream << req_header.first << ": ";
			req_stream << req_header.second << "\r\n";
		}

		req_stream << "\r\n";

		if (!data.empty())
		{
			req_stream << data;
		}
	}

	bool uri_conn::is_200(std::istream& res_stream)
	{
		std::string http_version;
		res_stream >> http_version;

		unsigned int status_code;
		res_stream >> status_code;

		std::string status_msg;
		std::getline(res_stream, status_msg);

		if (!res_stream || http_version.substr(0, 5) != "HTTP/")
		{
			return false;
		}

		if (status_code != 200)
		{
			return false;
		}

		return true;
	}

} // namespace waspp

