SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

CREATE SCHEMA IF NOT EXISTS `mydb_rent` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci ;
USE `mydb_rent` ;

-- -----------------------------------------------------
-- Table `mydb_rent`.`Client`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb_rent`.`Client` ;
CREATE TABLE IF NOT EXISTS `mydb_rent`.`Client` (
  `username` VARCHAR(16) NOT NULL,
  `email` VARCHAR(255) NULL,
  `password` VARCHAR(32) NOT NULL,
  `created_time` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `credit_no` BIGINT NULL,
  `idClient` INT,
  `gender` CHAR NULL,
  PRIMARY KEY (`username`));


-- -----------------------------------------------------
-- Table `mydb_rent`.`House`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb_rent`.`House`;
CREATE TABLE IF NOT EXISTS `mydb_rent`.`House` (
  idHouse INT NOT NULL,
  `ordered_status` TINYINT(1) NOT NULL,
  `price` FLOAT NOT NULL,
  `type` VARCHAR(45) NULL,
  `year` INT NULL,
  `locationHouse` VARCHAR(45) NOT NULL,
  `cap` VARCHAR(45) NULL,
  PRIMARY KEY (idHouse))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb_rent`.`Agency`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb_rent`.`Agency`;
CREATE TABLE IF NOT EXISTS `mydb_rent`.`Agency` (
  idAgency INT NOT NULL,
  `telephone` VARCHAR(11) NOT NULL,
  `locationAgency` VARCHAR(45) NOT NULL,
  PRIMARY KEY (idAgency))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `mydb_rent`.`Agency_has_House`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb_rent`.`Agency_has_House`;
CREATE TABLE IF NOT EXISTS `mydb_rent`.`Agency_has_House` (
  Agency_idAgency INT NOT NULL,
  idHouse INT NOT NULL,
  PRIMARY KEY (Agency_idAgency, idHouse),
  INDEX `fk_Agency_has_House_House1_idx` (idHouse ASC),
  INDEX `fk_Agency_has_House_Agency1_idx` (Agency_idAgency ASC),
  CONSTRAINT `fk_Agency_has_House_Agency1`
    FOREIGN KEY (Agency_idAgency)
    REFERENCES `mydb_rent`.`Agency` (idAgency)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Agency_has_House_House1`
    FOREIGN KEY (idHouse)
    REFERENCES `mydb_rent`.`House` (`idHouse`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB
DEFAULT CHARACTER SET = dec8;


-- -----------------------------------------------------
-- Table `mydb_rent`.`Client_rents_House`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb_rent`.`Client_rents_House`;
CREATE TABLE IF NOT EXISTS `mydb_rent`.`Client_rents_House` (
  `Client_username` VARCHAR(16) NOT NULL,
  `idHouse` INT NOT NULL,
  `duration` VARCHAR(45) NULL,
  `startDate` TIMESTAMP NULL,
  `cost` FLOAT NULL,
  PRIMARY KEY (`Client_username`, `idHouse`),
  INDEX `fk_Client_has_House_House1_idx` (`idHouse` ASC),
  INDEX `fk_Client_has_House_Client1_idx` (`Client_username` ASC),
  CONSTRAINT `fk_Client_has_House_Client1`
    FOREIGN KEY (`Client_username`)
    REFERENCES `mydb_rent`.`Client` (`username`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_Client_has_House_House1`
    FOREIGN KEY (`idHouse`)
    REFERENCES `mydb_rent`.`House` (`idHouse`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION);


-- -----------------------------------------------------
-- Table `mydb_rent`.`Visit`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `mydb_rent`.`Visit` ;

CREATE TABLE IF NOT EXISTS `mydb_rent`.`Visit` (
  `Client_username` VARCHAR(16) NOT NULL,
  `Agency_idAgency` INT NOT NULL,
  `House_idHouse` INT NOT NULL,
  `dateVisit` VARCHAR(20) NOT NULL,
  `locationHouse` VARCHAR(45) NULL);

SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;


insert into Agency values(1, '110', 'shanghai');
insert into Agency values(2, '911', 'beijing');

insert into House values(1, 0, 1000, '1b1b', 1, 'shanghai', '1');
insert into House values(2, 0, 1000, '1b1b', 2, 'shanghai', '1');
insert into House values(3, 0, 1000, '1b1b', 3, 'shanghai', '1');
insert into House values(4, 0, 1000, '1b1b', 1, 'shanghai', '1');
insert into House values(5, 0, 1000, '1b1b', 1, 'shanghai', '1');
insert into House values(6, 0, 1000, '1b1b', 1, 'shanghai', '1');
insert into House values(7, 0, 1000, '1b1b', 1, 'shanghai', '1');
insert into House values(8, 0, 1000, '1b1b', 1, 'shanghai', '1');
insert into House values(9, 0, 1000, '1b1b', 1, 'shanghai', '1');
insert into House values(10, 0, 1000, '1b1b', 1, 'shanghai', '1');

insert into House values(11, 0, 2000, '2b2b', 1, 'beijing', '2');
insert into House values(12, 0, 2000, '2b2b', 2, 'beijing', '2');
insert into House values(13, 0, 2000, '2b2b', 3, 'beijing', '2');
insert into House values(14, 0, 2000, '2b2b', 1, 'beijing', '2');
insert into House values(15, 0, 2000, '2b2b', 1, 'beijing', '2');
insert into House values(16, 0, 2000, '2b2b', 1, 'beijing', '2');
insert into House values(17, 0, 2000, '2b2b', 3, 'beijing', '2');
insert into House values(18, 0, 2000, '2b2b', 1, 'beijing', '2');
insert into House values(19, 0, 2000, '2b2b', 1, 'beijing', '2');
insert into House values(20, 0, 2000, '2b2b', 1, 'beijing', '2');

insert into House values(21, 0, 3000, '3b3b', 1, 'shanghai', '3');
insert into House values(22, 0, 3000, '3b3b', 2, 'shanghai', '3');
insert into House values(23, 0, 3000, '3b3b', 3, 'shanghai', '3');
insert into House values(24, 0, 3000, '3b3b', 1, 'shanghai', '3');
insert into House values(25, 0, 3000, '3b3b', 1, 'shanghai', '3');
insert into House values(26, 0, 3000, '3b3b', 1, 'shanghai', '3');
insert into House values(27, 0, 3000, '3b3b', 1, 'shanghai', '3');
insert into House values(28, 0, 3000, '3b3b', 1, 'shanghai', '3');
insert into House values(29, 0, 3000, '3b3b', 1, 'shanghai', '3');
insert into House values(30, 0, 3000, '3b3b', 1, 'shanghai', '3');

insert into House values(31, 0, 4000, '4b4b', 1, 'beijing', '4');
insert into House values(32, 0, 4000, '4b4b', 2, 'beijing', '4');
insert into House values(33, 0, 4000, '4b4b', 3, 'beijing', '4');
insert into House values(34, 0, 4000, '4b4b', 1, 'beijing', '4');
insert into House values(35, 0, 4000, '4b4b', 1, 'beijing', '4');
insert into House values(36, 0, 4000, '4b4b', 1, 'beijing', '4');
insert into House values(37, 0, 4000, '4b4b', 1, 'beijing', '4');
insert into House values(38, 0, 4000, '4b4b', 1, 'beijing', '4');
insert into House values(39, 0, 4000, '4b4b', 1, 'beijing', '4');
insert into House values(40, 0, 4000, '4b4b', 1, 'beijing', '4');

insert into Agency_has_House values(1, 1);
insert into Agency_has_House values(1, 2);
insert into Agency_has_House values(1, 3);
insert into Agency_has_House values(1, 4);
insert into Agency_has_House values(1, 5);
insert into Agency_has_House values(1, 6);
insert into Agency_has_House values(1, 7);
insert into Agency_has_House values(1, 8);
insert into Agency_has_House values(1, 9);
insert into Agency_has_House values(1, 10);

insert into Agency_has_House values(1, 21);
insert into Agency_has_House values(1, 22);
insert into Agency_has_House values(1, 23);
insert into Agency_has_House values(1, 24);
insert into Agency_has_House values(1, 25);
insert into Agency_has_House values(1, 26);
insert into Agency_has_House values(1, 27);
insert into Agency_has_House values(1, 28);
insert into Agency_has_House values(1, 29);
insert into Agency_has_House values(1, 30);

insert into Agency_has_House values(2, 11);
insert into Agency_has_House values(2, 12);
insert into Agency_has_House values(2, 13);
insert into Agency_has_House values(2, 14);
insert into Agency_has_House values(2, 15);
insert into Agency_has_House values(2, 16);
insert into Agency_has_House values(2, 17);
insert into Agency_has_House values(2, 18);
insert into Agency_has_House values(2, 19);
insert into Agency_has_House values(2, 20);

insert into Agency_has_House values(2, 31);
insert into Agency_has_House values(2, 32);
insert into Agency_has_House values(2, 33);
insert into Agency_has_House values(2, 34);
insert into Agency_has_House values(2, 35);
insert into Agency_has_House values(2, 36);
insert into Agency_has_House values(2, 37);
insert into Agency_has_House values(2, 38);
insert into Agency_has_House values(2, 39);
insert into Agency_has_House values(2, 40);
