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

#include <iostream>
#include <map>
#include <string>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "config.hpp"
#include "server.hpp"

int main(int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Usage: ./waspp config.json server0\n";
            return 1;
        }

        waspp::config c;
        if (!c.load(argv[1], argv[2]))
        {
            std::cerr << "config::load failed" << std::endl;
            return 1;
        }
		
        waspp::server s(c.address, c.port, c.doc_root, c.num_threads);
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}
