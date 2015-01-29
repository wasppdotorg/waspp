waspp
=====

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

Install Boost, MySQLCppConn and TCMalloc
----------------------------------------
* sudo apt-get install libboost-all-dev
* sudo apt-get install libmysqlcppconn-dev
* sudo apt-get install libgoogle-perftools-dev

Install php-mvc-framework
-------------------------
* cd /var/
* git clone https://github.com/waspporg/waspp.git
* cd waspp
* mkdir doc_root
* vi doc_root/index.html
```
waspp
```

* make
* sudo ./waspp config.json server0

Run
---
* Go to http://your.web.server:8000/
