
cmake_minimum_required (VERSION 2.8)

project (waspp)

find_package (Threads)
find_package (Boost 1.54.0 COMPONENTS system thread filesystem serialization iostreams)
find_library (MySQLClient_LIBRARY NAMES mysqlclient)
find_library (SSL_LIBRARY NAMES ssl)
find_library (Crypto_LIBRARY NAMES crypto)
find_library (HiRedis_LIBRARY NAMES hiredis)
find_library (TCMalloc_LIBRARY NAMES tcmalloc)

aux_source_directory (. Source_DIRECTORY)
add_executable (waspp ${Source_DIRECTORY})

if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "-O2")
	set (CMAKE_EXE_LINKER_FLAGS "-s")
endif()

target_link_libraries (waspp ${Threads_LIBRARY})
target_link_libraries (waspp ${Boost_LIBRARY})
target_link_libraries (waspp ${MySQLClient_LIBRARY})
target_link_libraries (waspp ${SSL_LIBRARY})
target_link_libraries (waspp ${Crypto_LIBRARY})
target_link_libraries (waspp ${HiRedis_LIBRARY})
target_link_libraries (waspp ${TCMalloc_LIBRARY})
