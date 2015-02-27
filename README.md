waspp
=====

Built with
----------
* CMake 2.8 - Ubuntu Linux 14.04 LTS
* Codeblocks 13.12 - Ubuntu Linux 14.04 LTS
* Visual Studio 2010 Express - Windows 7
* Visual Studio 2013 Professional - Windows 7

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

Install Boost, MariaDBClient, GnuTLS and TCMalloc
-------------------------------------------------
* sudo apt-get install libboost-all-dev
* sudo apt-get install libmariadbclient-dev
* sudo apt-get install libgnutls-dev
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

For Windows
-----------
* boost_1_56_0-msvc-12.0-64.exe (boost_1_54_0-msvc-10.0-32.exe)
* mariadb-10.0.16-winx64.msi (mariadb-10.0.16-win32.msi)
* Win64OpenSSL-1_0_1L.exe (Win32OpenSSL_Light-1_0_1L.exe)

Test
----
* sudo apt-get install libgtest-dev