
cmake_minimum_required (VERSION 2.8)

project (waspp)

find_package (Threads)
find_package (Boost 1.54.0 COMPONENTS system thread filesystem serialization iostreams)
find_library (MySQLClient_LIBRARIES NAMES mysqlclient)
find_library (SSL_LIBRARIES NAMES ssl)
find_library (Crypto_LIBRARIES NAMES crypto)
find_library (HiRedis_LIBRARIES NAMES hiredis)
find_library (TCMalloc_LIBRARIES NAMES tcmalloc)

aux_source_directory (. Source_DIRECTORIES)
add_executable (waspp ${Source_DIRECTORIES})

if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "-O2")
	#set (CMAKE_EXE_LINKER_FLAGS "-s")
endif()

target_link_libraries (waspp ${Threads_LIBRARIES})
target_link_libraries (waspp ${Boost_LIBRARIES})
target_link_libraries (waspp ${MySQLClient_LIBRARIES})
target_link_libraries (waspp ${SSL_LIBRARIES})
target_link_libraries (waspp ${Crypto_LIBRARIES})
target_link_libraries (waspp ${HiRedis_LIBRARIES})
target_link_libraries (waspp ${TCMalloc_LIBRARIES})
