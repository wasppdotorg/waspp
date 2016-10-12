/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef url_conn_hpp
#define url_conn_hpp

#include <curl/curl.h>

#include <string>

namespace waspp
{

	std::size_t curl_easy_write(char* ptr, std::size_t size, std::size_t nmemb, void* userdata);

	class url_conn
	{
	public:
		url_conn(const std::string& url);
		~url_conn();

		const std::string& content() { return content_; }

		bool query();

	private:
		CURL* curl;

		std::string url_;
		std::string content_;

	};

} // namespace waspp

#endif // url_conn_hpp
