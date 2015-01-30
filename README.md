waspp
=====

Tested with
-----------
* CMake on Ubuntu 14.04 LTS Linux Server (Boost 1.54)
* CodeBlocks 10.05 on Debian 7 Linux Desktop (Boost 1.49)
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

* sudo /etc/init.d/mysql restart

Install Boost, MySQLCppConn and TCMalloc
----------------------------------------
* sudo apt-get install libboost-all-dev
* sudo apt-get install libmysqlcppconn-dev
* sudo apt-get install libgoogle-perftools-dev

Install waspp
-------------
* cd /var/
* sudo git clone https://github.com/waspporg/waspp.git
* cd waspp
* sudo cmake .
* sudo make
* sudo ./waspp config.json server0

Run
---
* Go to http://your.web.server:8000/
