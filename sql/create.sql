
CREATE DATABASE IF NOT EXISTS `waspp_index` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE IF NOT EXISTS `waspp_000` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE IF NOT EXISTS `waspp_001` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
CREATE DATABASE IF NOT EXISTS `waspp_etc` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;

USE `waspp_index`;

	CREATE TABLE `unique_seq` (
	  `table_id` varchar(32) NOT NULL,
	  `incr_amount` int(11) NOT NULL,
	  `last_seq` int(11) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_seq` WRITE;
	INSERT INTO `unique_seq` VALUES ('users_index',1,1);
	UNLOCK TABLES;

	CREATE DEFINER=`root`@`%` PROCEDURE `USP_GET_UNIQUE_SEQ`(IN `p_table_id` VARCHAR(32), IN `p_seq_count` INT(11))
	BEGIN

	DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE EXIT HANDLER FOR SQLWARNING ROLLBACK;

	START TRANSACTION;

		UPDATE `unique_seq`
		SET `last_seq` = `last_seq` + (`incr_amount` * p_seq_count)
		WHERE `table_id` = p_table_id;

		SELECT `last_seq` 
		FROM `unique_seq`
		WHERE `table_id` = p_table_id; 

	COMMIT;

	END ;;
	
	CREATE TABLE `users_index` (
	  `userseq` int(11) unsigned NOT NULL,
	  `platformtype` int(11) NOT NULL,
	  `platformid` varchar(64) NOT NULL,
	  `username` varchar(64) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`userseq`),
	  UNIQUE KEY `platformid_UNIQUE` (`platformid`),
	  UNIQUE KEY `username_UNIQUE` (`username`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

USE `waspp_000`;

	CREATE TABLE `unique_seq` (
	  `table_id` varchar(32) NOT NULL,
	  `incr_amount` int(11) NOT NULL,
	  `last_seq` int(11) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_seq` WRITE;
	INSERT INTO `unique_seq` VALUES ('characters',2,0);
	INSERT INTO `unique_seq` VALUES ('char_inven',2,0);
	UNLOCK TABLES;

	CREATE DEFINER=`root`@`%` PROCEDURE `USP_GET_UNIQUE_SEQ`(IN `p_table_id` VARCHAR(32), IN `p_seq_count` INT(11))
	BEGIN

	DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE EXIT HANDLER FOR SQLWARNING ROLLBACK;

	START TRANSACTION;

		UPDATE `unique_seq`
		SET `last_seq` = `last_seq` + (`incr_amount` * p_seq_count)
		WHERE `table_id` = p_table_id;

		SELECT `last_seq` 
		FROM `unique_seq`
		WHERE `table_id` = p_table_id; 

	COMMIT;

	END ;;
	
	CREATE TABLE IF NOT EXISTS `users` (
	  `userseq` int(11) NOT NULL,
	  `passwd` varchar(32) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`userseq`, `userid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
	CREATE TABLE IF NOT EXISTS `characters` (
	  `charseq` int(11) unsigned NOT NULL,
	  `userseq` int(11) unsigned NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`charseq`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
	CREATE TABLE IF NOT EXISTS `char_inven` (
	  `invenseq` int(11) unsigned NOT NULL,
	  `charseq` int(11) unsigned NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`invenseq`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

USE `waspp_001`;

	CREATE TABLE `unique_seq` (
	  `table_id` varchar(32) NOT NULL,
	  `incr_amount` int(11) NOT NULL,
	  `last_seq` int(11) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_seq` WRITE;
	INSERT INTO `unique_seq` VALUES ('characters',2,1);
	INSERT INTO `unique_seq` VALUES ('char_inven',2,1);
	UNLOCK TABLES;

	CREATE DEFINER=`root`@`%` PROCEDURE `USP_GET_UNIQUE_SEQ`(IN `p_table_id` VARCHAR(32), IN `p_seq_count` INT(11))
	BEGIN

	DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE EXIT HANDLER FOR SQLWARNING ROLLBACK;

	START TRANSACTION;

		UPDATE `unique_seq`
		SET `last_seq` = `last_seq` + (`incr_amount` * p_seq_count)
		WHERE `table_id` = p_table_id;

		SELECT `last_seq` 
		FROM `unique_seq`
		WHERE `table_id` = p_table_id; 

	COMMIT;

	END ;;
	
	CREATE TABLE IF NOT EXISTS `users` (
	  `userseq` int(11) NOT NULL,
	  `passwd` varchar(32) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`userseq`, `userid`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
	CREATE TABLE IF NOT EXISTS `characters` (
	  `charseq` int(11) unsigned NOT NULL,
	  `userseq` int(11) unsigned NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`charseq`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
	
	CREATE TABLE IF NOT EXISTS `char_inven` (
	  `invenseq` int(11) unsigned NOT NULL,
	  `charseq` int(11) unsigned NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`invenseq`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

USE `waspp_etc`;

	CREATE TABLE `unique_seq` (
	  `table_id` varchar(16) NOT NULL,
	  `incr_amount` int(11) NOT NULL,
	  `last_seq` int(11) unsigned NOT NULL,
	  PRIMARY KEY (`table_id`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;

	LOCK TABLES `unique_seq` WRITE;
	INSERT INTO `unique_seq` VALUES ('board',1,1);
	UNLOCK TABLES;

	CREATE DEFINER=`root`@`%` PROCEDURE `USP_GET_UNIQUE_SEQ`(IN `p_table_id` VARCHAR(16), IN `p_seq_count` INT(11))
	BEGIN

	DECLARE EXIT HANDLER FOR SQLEXCEPTION ROLLBACK;
	DECLARE EXIT HANDLER FOR SQLWARNING ROLLBACK;

	START TRANSACTION;

		UPDATE `unique_seq`
		SET `last_seq` = `last_seq` + (`incr_amount` * p_seq_count)
		WHERE `table_id` = p_table_id;

		SELECT `last_seq` 
		FROM `unique_seq`
		WHERE `table_id` = p_table_id; 

	COMMIT;

	END ;;

	CREATE TABLE IF NOT EXISTS `board` (
	  `boardseq` int(11) NOT NULL,
	  `title` varchar(128) NOT NULL,
	  `content` text NOT NULL,
	  `file1` varchar(64) NOT NULL,
	  `file2` varchar(64) NOT NULL,
	  `userseq` int(11) NOT NULL,
	  `username` varchar(64) NOT NULL,
	  `inserttime` datetime NOT NULL,
	  `updatetime` datetime NOT NULL,
	  PRIMARY KEY (`boardseq`)
	) ENGINE=InnoDB DEFAULT CHARSET=utf8;
