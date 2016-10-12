/*
Copyright (c) 2014-present WASPP (waspp.org at gmail dot com)

Distributed under the Boost Software License, Version 1.0.
http://www.boost.org/LICENSE_1_0.txt
*/

#include <iostream>

#include "curl_easy.hpp"

namespace waspp
{

	std::size_t curl_easy_write_func(char* ptr, std::size_t size, std::size_t nmemb, void* userdata)
	{
		((std::string*)userdata)->append(ptr, size * nmemb);
		return size * nmemb;
	}

	curl_easy::curl_easy(const std::string& url)
		: curl(nullptr), url_(url)
	{
		curl = curl_easy_init();
	}

	curl_easy::~curl_easy()
	{
		if (curl)
		{
			curl_easy_cleanup(curl);
		}
	}

	bool curl_easy::perform()
	{
		if (!curl)
		{
			return false;
		}

		curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
		
		if (url_.substr(0, 5) == "https")
		{
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
		}
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_easy_write_func);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content_);

		CURLcode code = curl_easy_perform(curl);
		if (code == CURLE_OK)
		{
			return true;
		}

		return false;
	}

} // namespace waspp
