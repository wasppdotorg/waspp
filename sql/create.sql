
CREATE DATABASE IF NOT EXISTS `waspp` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `waspp`;

CREATE TABLE IF NOT EXISTS `board` (
  `seq` int(11) NOT NULL AUTO_INCREMENT,
  `title` varchar(50) NOT NULL,
  `content` text NOT NULL,
  `file1` varchar(50) NOT NULL,
  `file2` varchar(50) NOT NULL,
  `userid` varchar(15) NOT NULL,
  `inserttime` datetime NOT NULL,
  `updatetime` datetime NOT NULL,
  PRIMARY KEY (`seq`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

CREATE TABLE IF NOT EXISTS `user` (
  `userid` varchar(15) NOT NULL,
  `passwd` varchar(32) NOT NULL,
  `inserttime` datetime NOT NULL,
  `updatetime` datetime NOT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

