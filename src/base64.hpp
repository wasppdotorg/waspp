/* 
base64.cpp and base64.h

Copyright (C) 2004-2008 Rene Nyffenegger

This source code is provided 'as-is', without any express or implied
warranty. In no event will the author be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this source code must not be misrepresented; you must not
claim that you wrote the original source code. If you use this source code
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original source code.

3. This notice may not be removed or altered from any source distribution.

Rene Nyffenegger rene.nyffenegger@adp-gmbh.ch

*/

// this is not the original source code.
// added namespace base64
// and removed the base64_ prefix
// from function names and variable names.

#ifndef WASPP_BASE64_HPP
#define WASPP_BASE64_HPP

#include <string>

namespace waspp
{

	namespace base64
	{
		std::string encode(unsigned char const* , unsigned int len);
		std::string decode(std::string const& s);

	} // namespace base64

} // namespace waspp

#endif // WASPP_BASE64_HPP
