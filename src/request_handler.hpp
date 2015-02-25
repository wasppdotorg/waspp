//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_REQUEST_HANDLER_HPP
#define WASPP_REQUEST_HANDLER_HPP

#include <map>
#include <string>

#include <boost/noncopyable.hpp>

namespace waspp
{

	struct response;
	struct request;

	/// The common handler for all incoming requests.
	class request_handler
		: private boost::noncopyable
	{
	public:
		/// Construct with a directory containing files to be served.
		explicit request_handler(const std::string& doc_root_);

		/// Handle a request and produce a response.
		void handle_request(request& req, response& res);

	private:
		/// The directory containing the files to be served.
		std::string doc_root;

		/// Perform URL-decoding on a string. Returns false if the encoding was
		/// invalid.
		static bool url_validate_decode(const std::string& in, std::string& out);
	};

} // namespace waspp

#endif // WASPP_REQUEST_HANDLER_HPP
