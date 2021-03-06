
CREATE DATABASE `develop_waspp_000` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE `develop_waspp_001` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE `develop_waspp_etc` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

USE `develop_waspp_000`;

	CREATE TABLE `unique_keys` (
	  `table_id` varchar(32) NOT NULL,
	  `incr_amount` int(11) unsigned NOT NULL,
	  `last_key` bigint(20) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_keys` WRITE;
	INSERT INTO `unique_keys` VALUES ('users',2,0);
	INSERT INTO `unique_keys` VALUES ('characters',2,0);
	INSERT INTO `unique_keys` VALUES ('char_name',2,0);
	INSERT INTO `unique_keys` VALUES ('char_inven',2,0);
	UNLOCK TABLES;

	DELIMITER ;;
		CREATE DEFINER=`root`@`localhost` PROCEDURE `USP_GET_UNIQUE_KEYS`(IN `in_table_id` VARCHAR(32), IN `in_key_amount` INT(11) unsigned)
		BEGIN

		DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
		DECLARE EXIT HANDLER FOR SQLWARNING ROLLBACK;

		START TRANSACTION;

			UPDATE `unique_keys`
			SET `last_key` = `last_key` + (`incr_amount` * in_key_amount)
			WHERE `table_id` = in_table_id;

			SELECT `last_key` 
			FROM `unique_keys`
			WHERE `table_id` = in_table_id; 

		COMMIT;

		END ;;
	DELIMITER ;

	CREATE TABLE `users` (
	  `userid` bigint(20) unsigned NOT NULL,
	  `platformid` varchar(64) NOT NULL,
	  `platformtype` int(11) NOT NULL,
	  `username` varchar(64) NOT NULL,
	  `passwd` varchar(32) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`userid`),
	  UNIQUE KEY `platformid_UNIQUE` (`platformid`,`platformtype`),
	  UNIQUE KEY `username_UNIQUE` (`username`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	CREATE TABLE `characters` (
	  `charid` bigint(20) unsigned NOT NULL,
	  `userid` bigint(20) unsigned NOT NULL,
	  `charname` varchar(64) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`charid`,`userid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	CREATE TABLE `char_name` (
	  `charid` bigint(20) unsigned NOT NULL,
	  `userid` bigint(20) unsigned NOT NULL,
	  `charname` varchar(64) NOT NULL,
	  PRIMARY KEY (`charid`,`userid`),
	  UNIQUE KEY `charname_UNIQUE` (`charname`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	CREATE TABLE `char_inven` (
	  `invenseq` bigint(20) unsigned NOT NULL,
	  `charid` bigint(20) unsigned NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`invenseq`,`charid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

USE `develop_waspp_001`;

	CREATE TABLE `unique_keys` (
	  `table_id` varchar(32) NOT NULL,
	  `incr_amount` int(11) unsigned NOT NULL,
	  `last_key` bigint(20) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_keys` WRITE;
	INSERT INTO `unique_keys` VALUES ('users',2,1);
	INSERT INTO `unique_keys` VALUES ('characters',2,1);
	INSERT INTO `unique_keys` VALUES ('char_name',2,1);
	INSERT INTO `unique_keys` VALUES ('char_inven',2,1);
	UNLOCK TABLES;

	DELIMITER ;;
		CREATE DEFINER=`root`@`localhost` PROCEDURE `USP_GET_UNIQUE_KEYS`(IN `in_table_id` VARCHAR(32), IN `in_key_amount` INT(11) unsigned)
		BEGIN

		DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
		DECLARE EXIT HANDLER FOR SQLWARNING ROLLBACK;

		START TRANSACTION;

			UPDATE `unique_keys`
			SET `last_key` = `last_key` + (`incr_amount` * in_key_amount)
			WHERE `table_id` = in_table_id;

			SELECT `last_key` 
			FROM `unique_keys`
			WHERE `table_id` = in_table_id; 

		COMMIT;

		END ;;
	DELIMITER ;

	CREATE TABLE `users` (
	  `userid` bigint(20) unsigned NOT NULL,
	  `platformid` varchar(64) NOT NULL,
	  `platformtype` int(11) NOT NULL,
	  `username` varchar(64) NOT NULL,
	  `passwd` varchar(32) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`userid`),
	  UNIQUE KEY `platformid_UNIQUE` (`platformid`,`platformtype`),
	  UNIQUE KEY `username_UNIQUE` (`username`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	CREATE TABLE `characters` (
	  `charid` bigint(20) unsigned NOT NULL,
	  `userid` bigint(20) unsigned NOT NULL,
	  `charname` varchar(64) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`charid`,`userid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	CREATE TABLE `char_name` (
	  `charid` bigint(20) unsigned NOT NULL,
	  `userid` bigint(20) unsigned NOT NULL,
	  `charname` varchar(64) NOT NULL,
	  PRIMARY KEY (`charid`,`userid`),
	  UNIQUE KEY `charname_UNIQUE` (`charname`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	CREATE TABLE `char_inven` (
	  `invenseq` bigint(20) unsigned NOT NULL,
	  `charid` bigint(20) unsigned NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`invenseq`,`charid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

USE `develop_waspp_etc`;

	CREATE TABLE `unique_keys` (
	  `table_id` varchar(16) NOT NULL,
	  `incr_amount` int(11) unsigned NOT NULL,
	  `last_key` bigint(20) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_keys` WRITE;
	INSERT INTO `unique_keys` VALUES ('forum',1,1);
	UNLOCK TABLES;

	DELIMITER ;;
		CREATE DEFINER=`root`@`localhost` PROCEDURE `USP_GET_UNIQUE_KEYS`(IN `in_table_id` VARCHAR(32), IN `in_key_amount` INT(11) unsigned)
		BEGIN

		DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
		DECLARE EXIT HANDLER FOR SQLWARNING ROLLBACK;

		START TRANSACTION;

			UPDATE `unique_keys`
			SET `last_key` = `last_key` + (`incr_amount` * in_key_amount)
			WHERE `table_id` = in_table_id;

			SELECT `last_key` 
			FROM `unique_keys`
			WHERE `table_id` = in_table_id; 

		COMMIT;

		END ;;
	DELIMITER ;

	CREATE TABLE `forum` (
	  `seq` bigint(20) unsigned NOT NULL,
	  `title` varchar(128) NOT NULL,
	  `content` text NOT NULL,
	  `file1` varchar(64) NOT NULL,
	  `file2` varchar(64) NOT NULL,
	  `userid` bigint(20) unsigned NOT NULL,
	  `username` varchar(64) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`seq`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
