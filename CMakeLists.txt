cmake_minimum_required (VERSION 2.6)

project (waspp)

find_package(Threads)
find_package(Boost 1.54.0 COMPONENTS system thread) 
find_library(MySQLCppConn_LIBRARIES NAMES mysqlcppconn)
find_library(TCMalloc_LIBRARIES NAMES tcmalloc)

add_executable(waspp config.hpp connection.hpp database_mysql.hpp key_value.hpp mime_types.hpp request_handler.hpp request.hpp request_parser.hpp response.hpp router.hpp server.hpp config.cpp connection.cpp database_mysql.cpp main.cpp mime_types.cpp request_handler.cpp request_parser.cpp response.cpp route_board.cpp router.cpp server.cpp)

target_link_libraries(waspp ${Threads_LIBRARIES})
target_link_libraries(waspp ${Boost_LIBRARIES})
target_link_libraries(waspp ${MySQLCppConn_LIBRARIES})
target_link_libraries(waspp ${TCMalloc_LIBRARIES})
