waspp
=====

Tested with
-----------
* CMake on Ubuntu 14.04 LTS Linux Server (Boost 1.54)
* Visual Studio 2010 Express on Windows 7 (Boost 1.54 lib32-msvc-10.0)
* Visual Studio 2013 Professional on Windows 7 (Boost 1.55 lib64-msvc-12.0)


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
* sudo vi /etc/mysql/my.cnf
```
wait_timeout = 259200
```

* sudo vi /etc/mysql/conf.d/mariadb.cnf
```
[client]
default-character-set = utf8

[mysqld]
character-set-server  = utf8
collation-server      = utf8_general_ci
character_set_server   = utf8
collation_server       = utf8_general_ci
```

* sudo /etc/init.d/mysql restart
* mysql -u root -p

Install Boost, MySQLClient and TCMalloc
----------------------------------------
* sudo apt-get install libboost-all-dev
* sudo apt-get install libmariadbclient-dev
* sudo apt-get install libgoogle-perftools-dev

Install waspp
-------------
* cd /var/
* sudo apt-get install git
* sudo git clone https://github.com/waspporg/waspp.git
* cd waspp/src
* sudo apt-get install build-essential
* sudo apt-get install cmake
* sudo cmake .
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
* Go to http://your.web.server:8000/
