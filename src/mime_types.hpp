//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_MIME_TYPES_HPP
#define WASPP_MIME_TYPES_HPP

#include <string>

namespace waspp
{

	namespace mime_types
	{

		/// Convert a file extension into a MIME type.
		std::string extension_to_type(const std::string& extension);

		bool is_compressible(const std::string& extension);

	} // namespace mime_types

} // namespace waspp

#endif // WASPP_MIME_TYPES_HPP
