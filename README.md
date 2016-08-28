
waspp
=====

Build Environment
-----------------
* GCC 5.4 - Ubuntu 16.04 LTS (Boost 1.58)
* Visual Studio 2015 - Windows 10 (Boost 1.59)

Install MariaDB
---------------
* sudo apt-key adv --recv-keys --keyserver hkp://keyserver.ubuntu.com:80 0xF1656F24C74CD1D8
* sudo nano /etc/apt/sources.list
```
# http://downloads.mariadb.org/mariadb/repositories/
deb [arch=amd64,i386] http://ftp.kaist.ac.kr/mariadb/repo/10.1/ubuntu xenial main
deb-src http://ftp.kaist.ac.kr/mariadb/repo/10.1/ubuntu xenial main
```

* sudo apt-get update
* sudo apt-get upgrade
* sudo apt-get install mariadb-server
* sudo /etc/init.d/mysql restart

<!---
* sudo /etc/init.d/mysql stop
* sudo /usr/bin/mysqld_safe --skip-grant-tables &
* mysql -u root
```
update mysql.user set plugin='mysql_native_password';
quit;
```

* sudo kill -9 $(pgrep mysql)
* sudo /etc/init.d/mysql start
--->

Install MariaDB (Windows)
-------------------------
* mariadb-10.1.14-winx64.msi

<!--
Install HandlerSocket
---------------------
* sudo nano /etc/mysql/my.cnf
```
[mysqld]
..
handlersocket_address = 127.0.0.1
handlersocket_port = 9998
handlersocket_port_wr = 9999
```

* sudo mysql -u root -p
* INSTALL PLUGIN handlersocket SONAME 'handlersocket.so';
* exit
* sudo /etc/init.d/mysql restart
* sudo mysql -u root -p
* SHOW PROCESSLIST;
* exit
-->

Install Redis
-------------
* sudo apt-get update
* sudo apt-get upgrade
* sudo apt-get install redis-server
* sudo /etc/init.d/redis-server restart

Install Redis (Windows)
-----------------------
* Download redis-64.3.0.503.nupkg from https://www.nuget.org/packages/Redis-64/
* Rename redis-64.3.0.503.nupkg to redis-64.3.0.503.zip
* Extract redis-64.3.0.503.zip
* cd redis-64.3.0.503\tools
* .\redis-server.exe

Install Libraries
-----------------
* sudo apt-get install libboost-all-dev
* sudo apt-get install libmariadbclient-dev
* sudo apt-get install libssl-dev
* sudo apt-get install libhiredis-dev
* sudo apt-get install libgoogle-perftools-dev

Install Libraries (Windows)
---------------------------
* boost_1_59_0-msvc-14.0-64.exe
* Win64OpenSSL-1_0_2h.exe

Download waspp
--------------
* cd /var/
* sudo apt-get install git
* sudo git clone https://github.com/wasppdotorg/waspp.git

Create Databases
----------------
* sudo mysql -u root -p
```
source /var/waspp/sql/develop.sql
grant all privileges on develop_waspp_idx.* to 'waspp_dbuser'@'localhost' identified by 'waspp_passwd' with grant option;
grant all privileges on develop_waspp_000.* to 'waspp_dbuser'@'localhost' identified by 'waspp_passwd' with grant option;
grant all privileges on develop_waspp_001.* to 'waspp_dbuser'@'localhost' identified by 'waspp_passwd' with grant option;
grant all privileges on develop_waspp_etc.* to 'waspp_dbuser'@'localhost' identified by 'waspp_passwd' with grant option;
flush privileges;
select User, Host, Password from mysql.user;
exit
```

Configure waspp
---------------
* cd waspp/cfg
* mysql -u root -p
```
show variables like 'wait_timeout';
```

* grep wait_timeout develop.json
```
"wait_timeout_sec" : 600
```

* grep timeout /etc/redis/redis.conf
```
timeout 0
```

* grep timeout develop.json
```
"timeout_sec" : 0
```

* nano develop.json (if different)

Build waspp
-----------
* cd ../src
* sudo apt-get install build-essential
* sudo apt-get install cmake
* sudo mkdir cmake_debug
* cd cmake_debug
* sudo cmake -DCMAKE_BUILD_TYPE=Debug ..
* sudo make
* sudo mv -f ./waspp ../../bin/

Run
---
* cd ../../bin
* sudo chmod +x develop_start.sh
* sudo ./develop_start.sh
* sudo tail -f ../log/develop00.csv
* Go to http://127.0.0.1:10080/

Run (Windows)
-------------
* cd path\to\waspp
* copy src\x64\Debug\waspp.exe .\bin
* copy "C:\Program Files\Mariadb 10.1\lib\libmysql.dll" .\bin
* cd .\bin
* .\waspp.exe develop 00

Add Your Own Route
------------------
* cd ../src
* sudo cp route_forum.cpp route_notice.cpp
* sudo nano route_notice.cpp
* sudo nano router.hpp
```
namespace dir_notice
{
	void index_html(config* cfg, request& req, response& res);
	void index_jsonp(config* cfg, request& req, response& res);
	
} // namespace dir_notice
```

* sudo nano router.cpp
```
route routes[] =
{
	..
	{ "/notice/index/", &notice::index::html },
	{ "/_/notice/index/", &notice::index::jsonp },
	..
```

* cd cmake_debug
* sudo cmake ..
* sudo make

Enable SSL
----------
* cd ../ssl
* sudo openssl req -x509 -nodes -days 365 -newkey rsa:2048 -keyout ./develop.key -out ./develop.crt
* cd ../cfg
* sudo nano develop.json
```
"ssl" : 1,
```

Check Memory Leak
-----------------
* sudo apt-get install valgrind
* sudo valgrind ./waspp develop 00

Enable Core Dump
-------------------
* sudo sysctl -w kernel.core_pattern=core.%p
* sudo nano /etc/sysctl.d/20-core-pattern.conf 
```
kernel.core_pattern = core.%p
```

* ulimit -c unlimited
* sudo nano /etc/profile
```
ulimit -c unlimited
```
