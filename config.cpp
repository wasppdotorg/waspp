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

#include <map>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#include <boost/lexical_cast.hpp>

#include "config.hpp"

namespace waspp
{

bool config::load(const char* path, const char* item)
{
    try
    {
        std::ifstream is(path, std::ios::in | std::ios::binary);
        if (!is)
        {
            return false;
        }

        boost::property_tree::ptree pt;
        read_json(path, pt);

        BOOST_FOREACH(boost::property_tree::ptree::value_type const& item_, pt.get_child(""))
        {
            std::map<std::string, std::string> i;
            BOOST_FOREACH(boost::property_tree::ptree::value_type const& pair_, item_.second)
            {
                i.insert(std::make_pair(pair_.first, pair_.second.get_value<std::string>()));
            }

            c.insert(std::make_pair(item_.first, i));
        }

        std::map<std::string, std::map<std::string, std::string> >::iterator found = c.find(item);
        if (found == c.end())
        {
            return false;
        }

        if ((*found).second.find("address") == (*found).second.end() ||
            (*found).second.find("port") == (*found).second.end() ||
            (*found).second.find("doc_root") == (*found).second.end() ||
            (*found).second.find("num_thread") == (*found).second.end())
        {
            return false;
        }

        address = c[item]["address"];
        port = c[item]["port"];
        doc_root = c[item]["doc_root"];
        num_threads = boost::lexical_cast<std::size_t>(c[item]["num_threads"]);

        return true;
    }
    catch (std::exception& e)
    {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return false;
}

} // namespace waspp
