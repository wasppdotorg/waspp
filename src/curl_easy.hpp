/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#ifndef curl_easy_hpp
#define curl_easy_hpp

#include <curl/curl.h>

#include <string>

namespace waspp
{

	std::size_t curl_easy_write_func(char* ptr, std::size_t size, std::size_t nmemb, void* userdata);

	class curl_easy
	{
	public:
		curl_easy(const std::string& url);
		~curl_easy();

		const std::string& content() { return content_; }

		bool perform();

	private:
		CURL* curl;

		std::string url_;
		std::string content_;

	};

} // namespace waspp

#endif // curl_easy_hpp
