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
* sudo cmake
* sudo make
* sudo ./waspp config.json server0

Run
---
* Go to http://your.web.server:8000/
