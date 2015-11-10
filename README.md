waspp
=====

Build Environment
-----------------
* GCC 4.8 - Ubuntu Linux 14.04 LTS (Boost 1.54)
* Clang 3.4 - FreeBSD 10.2 (Boost 1.55)
* Visual Studio 2013 - Windows 7 (Boost 1.56)

<!--
* Xcode 6.1 - MacOS X 10.9
-->

Install MariaDB
---------------
* sudo apt-key adv --recv-keys --keyserver keyserver.ubuntu.com 0xcbcb082a1bb943db
* sudo nano /etc/apt/sources.list
```
deb http://ftp.osuosl.org/pub/mariadb/repo/10.0/ubuntu trusty main
deb-src http://ftp.osuosl.org/pub/mariadb/repo/10.0/ubuntu trusty main
```

* sudo apt-get update
* sudo apt-get upgrade
* sudo apt-get install mariadb-server
* sudo nano /etc/mysql/conf.d/mariadb.cnf
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

* mysql -u root -p
* INSTALL PLUGIN handlersocket SONAME 'handlersocket.so';
* exit
* sudo /etc/init.d/mysql restart
* mysql -u root -p
* SHOW PROCESSLIST;
* exit
-->

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

Install CCache
--------------
* sudo apt-get install ccache
* sudo ccache -C

Increase Open File Limit
------------------------
* ulimit -n
* sudo nano /etc/security/limits.conf
```
* hard nofile 65535
* soft nofile 65535
root hard nofile 65535
root soft nofile 65535
```

* exit
* ssh your@server
* ulimit -n

Download waspp
--------------
* cd /var/
* sudo apt-get install git
* sudo git clone https://github.com/waspporg/waspp.git

Create Databases
----------------
* mysql -u root -p
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
* grep wait_timeout /etc/mysql/my.cnf
```
wait_timeout = 600
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
* sudo mkdir cmakebuild
* cd cmakebuild
* sudo cmake ..
* sudo make
* sudo mv -f ./waspp ../../bin/

Run
---
* cd ../../bin
* sudo chmod +x develop_start.sh
* sudo ./develop_start.sh
* sudo tail -f ../log/develop00.csv
* Go to http://127.0.0.1:10080/

Run on Windows
--------------
* Run cmd.exe
```
cd path\to\waspp
copy src\x64\Debug\waspp.exe .\bin
copy src\x64\Debug\libmysql.dll .\bin
cd .\bin
.\waspp.exe develop 00

```

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

* cd cmakebuild
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

* cd ../src
* sudo nano connection.hpp server.hpp utility.hpp
```
#define CHECK_MEMORY_LEAK_WITH_SSL
```

* cd cmakebuild
* sudo make

Check Memory Leak
-----------------
* sudo apt-get install valgrind
* valgrind ./waspp develop 00

Enable Core Dump
-------------------
* sudo /etc/init.d/apport stop
* sudo nano /etc/default/apport 
```
enabled=0
```

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

* cd
* nano .gdbinit
```
python sys.path.append('/usr/share/gcc-4.8/python')
```

For FreeBSD
-----------
* sudo pkg install nano
* sudo pkg install ccache
* sudo pkg install boost-all
* sudo pkg install mariadb100-client
* sudo pkg install mariadb100-server
* sudo pkg install redis
* sudo pkg install hiredis
* sudo pkg install google-perftools

For Windows
-----------
* boost_1_56_0-msvc-12.0-64.exe
* mariadb-10.0.22-winx64.msi
* Win64OpenSSL-1_0_2d.exe

<!--
For MacOS X
-----------
* install macports (http://www.macports.org)
* sudo port install boost
* sudo port install mariadb
* sudo port install redis
* sudo port install openssl
* sudo port install hiredis
-->
