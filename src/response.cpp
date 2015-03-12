//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <string>

#include <boost/lexical_cast.hpp>

#include "response.hpp"

namespace waspp
{

	namespace http_status_strings
	{

		const std::string ok =
			"HTTP/1.1 200 OK\r\n";
		const std::string created =
			"HTTP/1.1 201 Created\r\n";
		const std::string accepted =
			"HTTP/1.1 202 Accepted\r\n";
		const std::string no_content =
			"HTTP/1.1 204 No Content\r\n";
		const std::string multiple_choices =
			"HTTP/1.1 300 Multiple Choices\r\n";
		const std::string moved_permanently =
			"HTTP/1.1 301 Moved Permanently\r\n";
		const std::string moved_temporarily =
			"HTTP/1.1 302 Moved Temporarily\r\n";
		const std::string not_modified =
			"HTTP/1.1 304 Not Modified\r\n";
		const std::string bad_request =
			"HTTP/1.1 400 Bad Request\r\n";
		const std::string unauthorized =
			"HTTP/1.1 401 Unauthorized\r\n";
		const std::string forbidden =
			"HTTP/1.1 403 Forbidden\r\n";
		const std::string not_found =
			"HTTP/1.1 404 Not Found\r\n";
		const std::string internal_server_error =
			"HTTP/1.1 500 Internal Server Error\r\n";
		const std::string not_implemented =
			"HTTP/1.1 501 Not Implemented\r\n";
		const std::string bad_gateway =
			"HTTP/1.1 502 Bad Gateway\r\n";
		const std::string service_unavailable =
			"HTTP/1.1 503 Service Unavailable\r\n";

		boost::asio::const_buffer to_buffer(response::http_status_type http_status)
		{
			switch (http_status)
			{
			case response::ok:
				return boost::asio::buffer(ok);
			case response::created:
				return boost::asio::buffer(created);
			case response::accepted:
				return boost::asio::buffer(accepted);
			case response::no_content:
				return boost::asio::buffer(no_content);
			case response::multiple_choices:
				return boost::asio::buffer(multiple_choices);
			case response::moved_permanently:
				return boost::asio::buffer(moved_permanently);
			case response::moved_temporarily:
				return boost::asio::buffer(moved_temporarily);
			case response::not_modified:
				return boost::asio::buffer(not_modified);
			case response::bad_request:
				return boost::asio::buffer(bad_request);
			case response::unauthorized:
				return boost::asio::buffer(unauthorized);
			case response::forbidden:
				return boost::asio::buffer(forbidden);
			case response::not_found:
				return boost::asio::buffer(not_found);
			case response::internal_server_error:
				return boost::asio::buffer(internal_server_error);
			case response::not_implemented:
				return boost::asio::buffer(not_implemented);
			case response::bad_gateway:
				return boost::asio::buffer(bad_gateway);
			case response::service_unavailable:
				return boost::asio::buffer(service_unavailable);
			default:
				return boost::asio::buffer(internal_server_error);
			}
		}

	} // namespace http_status_strings

	namespace misc_strings
	{

		const char name_value_separator[] = { ':', ' ' };
		const char crlf[] = { '\r', '\n' };

	} // namespace misc_strings

	std::vector<boost::asio::const_buffer> response::to_buffers()
	{
		std::vector<boost::asio::const_buffer> buffers;
		buffers.push_back(http_status_strings::to_buffer(http_status));
		for (std::size_t i = 0; i < headers.size(); ++i)
		{
			name_value& p = headers[i];
			buffers.push_back(boost::asio::buffer(p.name));
			buffers.push_back(boost::asio::buffer(misc_strings::name_value_separator));
			buffers.push_back(boost::asio::buffer(p.value));
			buffers.push_back(boost::asio::buffer(misc_strings::crlf));
		}
		buffers.push_back(boost::asio::buffer(misc_strings::crlf));
		buffers.push_back(boost::asio::buffer(content));
		return buffers;
	}

	namespace static_responses
	{

		const char ok[] = "";
		const char created[] =
			"<html>"
			"<head><title>Created</title></head>"
			"<body><h1>201 Created</h1></body>"
			"</html>";
		const char accepted[] =
			"<html>"
			"<head><title>Accepted</title></head>"
			"<body><h1>202 Accepted</h1></body>"
			"</html>";
		const char no_content[] =
			"<html>"
			"<head><title>No Content</title></head>"
			"<body><h1>204 Content</h1></body>"
			"</html>";
		const char multiple_choices[] =
			"<html>"
			"<head><title>Multiple Choices</title></head>"
			"<body><h1>300 Multiple Choices</h1></body>"
			"</html>";
		const char moved_permanently[] =
			"<html>"
			"<head><title>Moved Permanently</title></head>"
			"<body><h1>301 Moved Permanently</h1></body>"
			"</html>";
		const char moved_temporarily[] =
			"<html>"
			"<head><title>Moved Temporarily</title></head>"
			"<body><h1>302 Moved Temporarily</h1></body>"
			"</html>";
		const char not_modified[] =
			"<html>"
			"<head><title>Not Modified</title></head>"
			"<body><h1>304 Not Modified</h1></body>"
			"</html>";
		const char bad_request[] =
			"<html>"
			"<head><title>Bad Request</title></head>"
			"<body><h1>400 Bad Request</h1></body>"
			"</html>";
		const char unauthorized[] =
			"<html>"
			"<head><title>Unauthorized</title></head>"
			"<body><h1>401 Unauthorized</h1></body>"
			"</html>";
		const char forbidden[] =
			"<html>"
			"<head><title>Forbidden</title></head>"
			"<body><h1>403 Forbidden</h1></body>"
			"</html>";
		const char not_found[] =
			"<html>"
			"<head><title>Not Found</title></head>"
			"<body><h1>404 Not Found</h1></body>"
			"</html>";
		const char internal_server_error[] =
			"<html>"
			"<head><title>Internal Server Error</title></head>"
			"<body><h1>500 Internal Server Error</h1></body>"
			"</html>";
		const char not_implemented[] =
			"<html>"
			"<head><title>Not Implemented</title></head>"
			"<body><h1>501 Not Implemented</h1></body>"
			"</html>";
		const char bad_gateway[] =
			"<html>"
			"<head><title>Bad Gateway</title></head>"
			"<body><h1>502 Bad Gateway</h1></body>"
			"</html>";
		const char service_unavailable[] =
			"<html>"
			"<head><title>Service Unavailable</title></head>"
			"<body><h1>503 Service Unavailable</h1></body>"
			"</html>";

		std::string to_string(response::http_status_type http_status)
		{
			switch (http_status)
			{
			case response::ok:
				return ok;
			case response::created:
				return created;
			case response::accepted:
				return accepted;
			case response::no_content:
				return no_content;
			case response::multiple_choices:
				return multiple_choices;
			case response::moved_permanently:
				return moved_permanently;
			case response::moved_temporarily:
				return moved_temporarily;
			case response::not_modified:
				return not_modified;
			case response::bad_request:
				return bad_request;
			case response::unauthorized:
				return unauthorized;
			case response::forbidden:
				return forbidden;
			case response::not_found:
				return not_found;
			case response::internal_server_error:
				return internal_server_error;
			case response::not_implemented:
				return not_implemented;
			case response::bad_gateway:
				return bad_gateway;
			case response::service_unavailable:
				return service_unavailable;
			default:
				return internal_server_error;
			}
		}

	} // namespace static_responses

	response response::static_response(response::http_status_type http_status)
	{
		response res;
		res.http_status = http_status;
		res.content = static_responses::to_string(http_status);

		res.headers.resize(3);
		res.headers[0].name = "Content-Length";
		res.headers[0].value = boost::lexical_cast<std::string>(res.content.size());
		res.headers[1].name = "Content-Type";
		res.headers[1].value = "text/html";
		res.headers[2].name = "Keep-Alive";
		res.headers[2].value = "timeout=0, max=0";

		return res;
	}

} // namespace waspp

