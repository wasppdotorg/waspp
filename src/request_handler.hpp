//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_REQUEST_HANDLER_HPP
#define WASPP_REQUEST_HANDLER_HPP

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
		explicit request_handler();

		/// Handle a request and produce a response.
		void handle_request(request& req, response& res);

	private:
		
	};

} // namespace waspp

#endif // WASPP_REQUEST_HANDLER_HPP
