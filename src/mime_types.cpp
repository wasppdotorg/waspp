//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "mime_types.hpp"

namespace waspp
{
	namespace mime_types
	{

		struct mapping
		{
			const char* extension;
			const char* mime_type;
		};

		mapping mappings[] =
		{
			{ "css", "text/css" },
			{ "gif", "image/gif" },
			{ "htm", "text/html" },
			{ "html", "text/html" },
			{ "ico", "image/x-icon" },
			{ "jpg", "image/jpeg" },
			{ "js", "application/javascript" },
			{ "json", "application/json" },
			{ "png", "image/png" },
			{ "txt", "text/plain" },
			{ "xhtml", "text/html" },
			{ "xml", "application/xml" },
			{ 0, 0 } // Marks end of list.
		};

		std::string extension_to_type(const std::string& extension)
		{
			if (extension.empty())
			{
				return "application/octet-stream";
			}

			for (mapping* m = mappings; m->extension; ++m)
			{
				if (m->extension == extension)
				{
					return m->mime_type;
				}
			}

			return "application/octet-stream";
		}

		const char* compressible[] = { "css", "htm", "html", "js", "json", "txt", "xhtml", "xml", 0 };
		
		bool is_compressible(const std::string& extension)
		{
			int i = 0;
			while (compressible[i] != '\0')
			{
				if (compressible[i] == extension)
				{
					return true;
				}

				++i;
			}

			return false;
		}

	} // namespace mime_types

} // namespace waspp
