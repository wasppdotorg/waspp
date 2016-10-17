//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef request_handler_hpp
#define request_handler_hpp

#include "request.hpp"
#include "response.hpp"
#include "config.hpp"

namespace waspp
{

	/// The common handler for all incoming requests.
	class request_handler
	{
	public:
		request_handler(const request_handler&) = delete;
		request_handler& operator=(const request_handler&) = delete;

		/// Construct with a directory containing files to be served.
		request_handler();

		/// Handle a request and produce a response.
		void handle_request(request& req, response& res);

	private:
		bool is_access_granted(const std::string& remote_addr);
		bool is_access_denied(const std::string& remote_addr);

		/// Perform URL-decoding on a string. Returns false if the encoding was
		/// invalid.
		static bool percent_decode_and_validate(const std::string& in, std::string& out);

		config& cfg;

	};

} // namespace waspp

#endif // request_handler_hpp
