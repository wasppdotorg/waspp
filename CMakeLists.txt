cmake_minimum_required (VERSION 2.6)

project (waspp)

find_package(Threads)
find_package(Boost 1.54.0 COMPONENTS system thread filesystem) 
find_library(MySQLClient_LIBRARIES NAMES mysqlclient)
find_library(TCMalloc_LIBRARIES NAMES tcmalloc)

add_executable(waspp config.hpp connection.hpp database_pool.hpp key_value.hpp logger.hpp mime_types.hpp mysqlpp.hpp request_handler.hpp request.hpp request_parser.hpp response.hpp router.hpp server.hpp singleton.hpp spinlock.hpp config.cpp connection.cpp database_pool.cpp logger.cpp main.cpp mime_types.cpp mysqlpp_connection.cpp mysqlpp_result.cpp mysqlpp_statement.cpp request_handler.cpp request_parser.cpp response.cpp route_board.cpp router.cpp server.cpp)

target_link_libraries(waspp ${Threads_LIBRARIES})
target_link_libraries(waspp ${Boost_LIBRARIES})
target_link_libraries(waspp ${MySQLClient_LIBRARIES})
target_link_libraries(waspp ${TCMalloc_LIBRARIES})
