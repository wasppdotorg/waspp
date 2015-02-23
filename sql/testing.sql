
CREATE DATABASE `testing_waspp_idx` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE `testing_waspp_000` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE `testing_waspp_001` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE `testing_waspp_etc` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

USE `testing_waspp_idx`;

	CREATE TABLE `unique_keys` (
	  `table_id` varchar(32) NOT NULL,
	  `incr_amount` int(11) NOT NULL,
	  `last_key` int(11) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_keys` WRITE;
	INSERT INTO `unique_keys` VALUES ('users_idx',1,1);
	UNLOCK TABLES;

	DELIMITER ;;
		CREATE DEFINER=`root`@`localhost` PROCEDURE `USP_GET_UNIQUE_KEYS`(IN `in_table_id` VARCHAR(32), IN `in_key_amount` INT(11))
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
	
	CREATE TABLE `users_idx` (
	  `userid` int(11) unsigned NOT NULL,
	  `platformtype` int(11) NOT NULL,
	  `platformid` varchar(64) NOT NULL,
	  `username` varchar(64) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`userid`),
	  UNIQUE KEY `platformid_UNIQUE` (`platformid`),
	  UNIQUE KEY `username_UNIQUE` (`username`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

USE `testing_waspp_000`;

	CREATE TABLE `unique_keys` (
	  `table_id` varchar(32) NOT NULL,
	  `incr_amount` int(11) NOT NULL,
	  `last_key` int(11) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_keys` WRITE;
	INSERT INTO `unique_keys` VALUES ('characters',2,0);
	INSERT INTO `unique_keys` VALUES ('char_inven',2,0);
	UNLOCK TABLES;

	DELIMITER ;;
		CREATE DEFINER=`root`@`localhost` PROCEDURE `USP_GET_UNIQUE_KEYS`(IN `in_table_id` VARCHAR(32), IN `in_key_amount` INT(11))
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
	  `userid` int(11) NOT NULL,
	  `passwd` varchar(32) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`userid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
	CREATE TABLE `characters` (
	  `charid` int(11) unsigned NOT NULL,
	  `userid` int(11) unsigned NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`charid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
	CREATE TABLE `char_inven` (
	  `invenseq` int(11) unsigned NOT NULL,
	  `charid` int(11) unsigned NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`invenseq`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

USE `testing_waspp_001`;

	CREATE TABLE `unique_keys` (
	  `table_id` varchar(32) NOT NULL,
	  `incr_amount` int(11) NOT NULL,
	  `last_key` int(11) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_keys` WRITE;
	INSERT INTO `unique_keys` VALUES ('characters',2,1);
	INSERT INTO `unique_keys` VALUES ('char_inven',2,1);
	UNLOCK TABLES;

	DELIMITER ;;
		CREATE DEFINER=`root`@`localhost` PROCEDURE `USP_GET_UNIQUE_KEYS`(IN `in_table_id` VARCHAR(32), IN `in_key_amount` INT(11))
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
	  `userid` int(11) NOT NULL,
	  `passwd` varchar(32) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`userid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
	CREATE TABLE `characters` (
	  `charid` int(11) unsigned NOT NULL,
	  `userid` int(11) unsigned NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`charid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
	CREATE TABLE `char_inven` (
	  `invenseq` int(11) unsigned NOT NULL,
	  `charid` int(11) unsigned NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`invenseq`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

USE `testing_waspp_etc`;

	CREATE TABLE `unique_keys` (
	  `table_id` varchar(16) NOT NULL,
	  `incr_amount` int(11) NOT NULL,
	  `last_key` int(11) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_keys` WRITE;
	INSERT INTO `unique_keys` VALUES ('board',1,1);
	UNLOCK TABLES;

	DELIMITER ;;
		CREATE DEFINER=`root`@`localhost` PROCEDURE `USP_GET_UNIQUE_KEYS`(IN `in_table_id` VARCHAR(32), IN `in_key_amount` INT(11))
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

	CREATE TABLE `board` (
	  `boardseq` int(11) NOT NULL,
	  `title` varchar(128) NOT NULL,
	  `content` text NOT NULL,
	  `file1` varchar(64) NOT NULL,
	  `file2` varchar(64) NOT NULL,
	  `userid` int(11) NOT NULL,
	  `username` varchar(64) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`boardseq`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
