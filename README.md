waspp
=====

Build Tested With
-----------------
* CMake 2.8 - Ubuntu Linux 14.04 LTS
* Codeblocks 13.12 - Ubuntu Linux 14.04 LTS
* Visual Studio 2010 Express - Windows 7 (Boost 1.54)
* Visual Studio 2013 Professional - Windows 7
* Xcode 6.1 - MacOS X 10.9

Install MariaDB
---------------
* sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 0xcbcb082a1bb943db
* sudo vi /etc/apt/sources.list
```
deb http://ftp.osuosl.org/pub/mariadb/repo/10.0/ubuntu trusty main
deb-src http://ftp.osuosl.org/pub/mariadb/repo/10.0/ubuntu trusty main
```

* sudo apt-get update
* sudo apt-get upgrade
* sudo apt-get install mariadb-server
* sudo vi /etc/mysql/conf.d/mariadb.cnf
```
[client]
default-character-set = utf8

[mysqld]
character-set-server = utf8
collation-server     = utf8_general_ci
character_set_server = utf8
collation_server     = utf8_general_ci
```

* sudo /etc/init.d/mysql restart

Install Redis
-------------
* sudo apt-get update
* sudo apt-get upgrade
* sudo apt-get install redis-server
* sudo /etc/init.d/redis-server restart

Install Libraries
-----------------
* sudo apt-get install libboost-all-dev
* sudo apt-get install libmariadbclient-dev
* sudo apt-get install libssl-dev
* sudo apt-get install libgnutls-dev
* sudo apt-get install libhiredis-dev
* sudo apt-get install libgoogle-perftools-dev

Increase Open File Limit
------------------------
* ulimit -n
* sudo vi /etc/sysctl.conf
```
fs.file-max = 65536
```

* sudo vi /etc/security/limits.conf
```
* hard nofile 65535
* soft nofile 65535
root hard nofile 65535
root soft nofile 65535
```

* exit
* ssh your@server
* ulimit -n

Install waspp
-------------
* cd /var/
* sudo apt-get install git
* sudo git clone https://github.com/waspporg/waspp.git
* cd waspp/cfg
* cat /etc/mysql/my.cnf | grep wait_timeout
```
wait_timeout = 28800
```

* cat *.json | grep timeout_sec
```
"timeout_sec" : 28800
```

* cd ../src
* sudo apt-get install build-essential
* sudo apt-get install cmake
* sudo cmake . (look, there's a dot :)
* sudo make
* mysql -u root -p
```
source /var/waspp/sql/develop.sql
```

* sudo mv -f ./waspp ../bin/

Run
---
* cd ../bin
* sudo ./develop_start.sh
* sudo tail -f ../log/develop00.csv
* Go to http://127.0.0.1:10080/

Run on Windows
--------------
* Run cmd.exe
```
cd path\to\waspp

copy src\x64\Debug\waspp_vs2013_x64.exe .\bin
(copy src\Debug\waspp_vs2010_win32.exe .\bin)

copy src\x64\Debug\libmysql.dll .\bin
(copy src\Debug\libmysql.dll .\bin)

cd .\bin

.\waspp_vs2013_x64.exe develop 00
(.\waspp_vs2010_win32.exe develop 00)
```

Add Your Own Route
------------------
* cd ../src
* sudo cp route_forum.cpp route_notice.cpp
* sudo vi route_notice.cpp
* sudo vi router.hpp
```
namespace dir_notice
{
	void index_html(logger* log, config* cfg, database* db, request& req, response& res);
	void index_jsonp(logger* log, config* cfg, database* db, request& req, response& res);
	
} // namespace dir_notice
```

* sudo vi router.cpp
```
route routes[] =
{
	..
	{ "/notice/index/", &notice::index::html },
	{ "/_/notice/index/", &notice::index::jsonp },
	..
```

* sudo cmake .
* sudo make

Enable SSL
----------
* cd ../ssl
* sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout ./develop.key -out ./develop.crt
* cd ../src
* sudo vi connection.hpp server.hpp utility.hpp
```
#define CHECK_MEMORY_LEAK_WITH_SSL
```

* sudo make
* cd ../cfg
* sudo vi develop.json
```
"ssl" : 1,
```

Disable TCP Slow Start For HTTP/2
---------------------------------
* sudo sysctl net.ipv4.tcp_slow_start_after_idle
* sudo sysctl -w net.ipv4.tcp_slow_start_after_idle=0
* sudo sysctl net.ipv4.tcp_slow_start_after_idle

Enable HTTP/2 (still working on)
--------------------------------
* enable ssl first
* sudo vi develop.json
```
"http2" : 1
```

Check Memory Leak
-----------------
* sudo apt-get install valgrind
* valgrind ./waspp develop 00

<!--
Test with Google Test
---------------------
* sudo apt-get install libgtest-dev
* cd /usr/src/gtest
* sudo cmake CMakeLists.txt
* sudo make
* sudo cp *.a /usr/lib
-->

For Windows
-----------
* boost_1_56_0-msvc-12.0-64.exe (boost_1_54_0-msvc-10.0-32.exe)
* mariadb-10.0.16-winx64.msi (mariadb-10.0.16-win32.msi)
* Win64OpenSSL-1_0_2a.exe (Win32OpenSSL-1_0_2a.exe)

For MacOS X
-----------
* install macports (http://www.macports.org)
* sudo port install boost
* sudo port install mariadb
* sudo port install redis
* sudo port install openssl
