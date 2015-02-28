
cmake_minimum_required (VERSION 2.8)

project (waspp)

find_package (Threads)
find_package (Boost 1.54.0 COMPONENTS system thread filesystem serialization)
find_library (MySQLClient_LIBRARIES NAMES mysqlclient)
find_library (Crypto_LIBRARIES NAMES crypto)
find_library (TCMalloc_LIBRARIES NAMES tcmalloc)

add_executable (

	waspp
	
	base64.hpp
	config.hpp
	connection.hpp
	database.hpp
	dbconn_pool.hpp
	key_value.hpp
	logger.hpp
	mime_types.hpp
	mysqlpp.hpp
	request_handler.hpp
	request.hpp
	request_parser.hpp
	response.hpp
	router.hpp
	server.hpp
	session.hpp
	utility.hpp
	
	base64.cpp
	config.cpp
	connection.cpp
	database.cpp
	dbconn_pool.cpp
	logger.cpp
	main.cpp
	mime_types.cpp
	mysqlpp_connection.cpp
	mysqlpp_result.cpp
	mysqlpp_statement.cpp
	request_handler.cpp
	request_parser.cpp
	response.cpp
	
	route_board.cpp
	router.cpp
	
	server.cpp
	session.cpp

)

target_link_libraries (waspp ${Threads_LIBRARIES})
target_link_libraries (waspp ${Boost_LIBRARIES})
target_link_libraries (waspp ${MySQLClient_LIBRARIES})
target_link_libraries (waspp ${Crypto_LIBRARIES})
target_link_libraries (waspp ${TCMalloc_LIBRARIES})

