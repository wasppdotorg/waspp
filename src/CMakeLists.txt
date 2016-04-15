
cmake_minimum_required (VERSION 2.8)

set (TARGET waspp)
project (${TARGET})

#find_program (CCACHE_FOUND ccache)
#if (CCACHE_FOUND)
    #set_property (GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    #set_property (GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
#endif()

add_definitions(-std=c++11)
aux_source_directory (. Source_DIRECTORIES)
add_executable (${TARGET} ${Source_DIRECTORIES})

include_directories (/usr/include /usr/local/include)

find_package (Threads)
find_package (Boost 1.55.0 COMPONENTS system thread filesystem chrono serialization iostreams)

find_library (MySQLClient_LIBRARIES NAMES mysqlclient PATHS /usr/lib/mysql /usr/local/lib/mysql)
find_library (HiRedis_LIBRARIES NAMES hiredis)
find_library (SSL_LIBRARIES NAMES ssl)
find_library (Crypto_LIBRARIES NAMES crypto)
#find_library (TCMalloc_LIBRARIES NAMES tcmalloc)

if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "-Wall -O2 -g")
	#set (CMAKE_EXE_LINKER_FLAGS "-s")
endif()

target_link_libraries (${TARGET} ${Threads_LIBRARIES})
target_link_libraries (${TARGET} ${Boost_LIBRARIES})
target_link_libraries (${TARGET} ${MySQLClient_LIBRARIES})
target_link_libraries (${TARGET} ${HiRedis_LIBRARIES})
target_link_libraries (${TARGET} ${SSL_LIBRARIES})
target_link_libraries (${TARGET} ${Crypto_LIBRARIES})
#target_link_libraries (${TARGET} ${TCMalloc_LIBRARIES})

if (${CMAKE_SYSTEM_NAME} MATCHES FreeBSD)
	target_link_libraries (${TARGET} "-lthr")
endif()

