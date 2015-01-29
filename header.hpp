//
// Copyright (c) 2003-2012 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// WASPP : Web Application Server written in C++
//
// Copyright (c) 2014-2015 WASPP (waspp.org@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef WASPP_HEADER_HPP
#define WASPP_HEADER_HPP

#include <string>

namespace waspp
{

struct header
{
    std::string name;
    std::string value;
};

} // namespace waspp

#endif // WASPP_HEADER_HPP
