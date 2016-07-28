
cmake_minimum_required (VERSION 3.0)

set (TARGET waspp)
project (${TARGET})

if (NOT CMAKE_BUILD_TYPE)
	message ("")
	message ("cmake -DCMAKE_BUILD_TYPE=Debug ..")
	message ("cmake -DCMAKE_BUILD_TYPE=Release ..")
	message ("")

	message (FATAL_ERROR "CMAKE_BUILD_TYPE NOT SPECIFIED")
endif()

#find_program (CCACHE_FOUND ccache)
#if (CCACHE_FOUND)
#	set_property (GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
#	set_property (GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
#endif()

aux_source_directory (. Source_DIRECTORIES)
add_executable (${TARGET} ${Source_DIRECTORIES})

include_directories (/usr/include /usr/local/include)

find_library (POSIXThread_LIBRARIES NAMES pthread)
find_library (MySQLClient_LIBRARIES NAMES mysqlclient PATHS /usr/lib/mysql /usr/local/lib/mysql)
find_library (HiRedis_LIBRARIES NAMES hiredis)
find_library (SSL_LIBRARIES NAMES ssl)
find_library (Crypto_LIBRARIES NAMES crypto)
find_package (Boost 1.55.0 COMPONENTS system filesystem serialization iostreams)

if (CMAKE_BUILD_TYPE STREQUAL Release)
	find_library (TCMalloc_LIBRARIES NAMES tcmalloc)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES Clang)
	set (CMAKE_COMPILER_IS_CLANGCXX 1)
endif()

if (CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANGCXX)
	set (CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -Wall -g -fexceptions")
	set (CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -Wall -g -fexceptions -O2 -DNDEBUG")
endif()

target_link_libraries (${TARGET} ${POSIXThread_LIBRARIES})
target_link_libraries (${TARGET} ${MySQLClient_LIBRARIES})
target_link_libraries (${TARGET} ${HiRedis_LIBRARIES})
target_link_libraries (${TARGET} ${SSL_LIBRARIES})
target_link_libraries (${TARGET} ${Crypto_LIBRARIES})
target_link_libraries (${TARGET} ${Boost_LIBRARIES})

if (CMAKE_BUILD_TYPE STREQUAL Release)
	target_link_libraries (${TARGET} ${TCMalloc_LIBRARIES})
endif()

