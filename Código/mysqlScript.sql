CREATE TABLE data(
date INT NOT NULL,
central VARCHAR(120) NOT NULL,
var1 FLOAT,
var2 FLOAT,
var3 FLOAT,
var4 FLOAT,
var5 FLOAT,
var6 FLOAT,
var10 FLOAT,
var11 FLOAT,
var12 FLOAT,
var13 FLOAT,
var14 FLOAT,
var15 FLOAT,
primary key (date, central)
);

CREATE TABLE dataMonth(
date INT NOT NULL,
dateCompact VARCHAR(120) NOT NULL,
central VARCHAR(120) NOT NULL,
var1 FLOAT,
var2 FLOAT,
var3 FLOAT,
var4 FLOAT,
var5 FLOAT,
var6 FLOAT,
var7 FLOAT,
var8 FLOAT,
var9 FLOAT,
primary key (dateCompact, central)
);

CREATE TABLE dataYear(
date INT NOT NULL,
dateCompact VARCHAR(120) NOT NULL,
central VARCHAR(120) NOT NULL,
var1 FLOAT,
var2 FLOAT,
var3 FLOAT,
var4 FLOAT,
var5 FLOAT,
var6 FLOAT,
var7 FLOAT,
var8 FLOAT,
var9 FLOAT,
primary key (dateCompact, central)
);

CREATE TABLE central (
	id int(11) NOT NULL auto_increment,
	central varchar(20) NOT NULL,
	power float NOT NULL,
	inversor float NOT NULL,
	time float NOT NULL,
	PRIMARY KEY (id),
	UNIQUE KEY central (central)
);

CREATE TABLE users (
	id int(11) NOT NULL auto_increment,
	username varchar(20) NOT NULL,
	password char(40) NOT NULL,
	PRIMARY KEY (id),
	UNIQUE KEY username (username)
);

DELIMITER $$ 
CREATE 
EVENT `archive_data` 
ON SCHEDULE EVERY 1 HOUR STARTS "2000-01-01 00:00:00" 
DO BEGIN 
TRUNCATE TABLE dataYear; 
INSERT INTO dataYear (central, date, var1,var2,var3,var4,var5,var6,var7,var8,var9)   
SELECT DISTINCT central, CONCAT( DATE_FORMAT(from_unixtime(`date`), '%Y'), ":", DATE_FORMAT(from_unixtime(`date`), '%m')) dateInYear, 
ROUND(AVG(var1),1) `Tension AC Media`,
ROUND((SUM(var3)/(60*(60/(86400/COUNT(var3)))))/1000,1) as `Energia AC generada`,
ROUND(AVG(var6),1) as `Frecuencia`, 
ROUND(SUM(var12)/(60*(60/(86400/COUNT(var12))))/1000,1) as `Energia DC generada`, 
ROUND(SUM(var13)/(60*(60/(86400/COUNT(var13))))/1000,1) as `Irradiacion total`, 
ROUND(AVG(var15),1) as `Temp. ambiente media`,
SUM(var3)/(60*(60/(86400/COUNT(var3))))/1000 as `EACG`, 
SUM(var12)/(60*(60/(86400/COUNT(var12))))/1000 as `EDCG`, 
SUM(var13)/(60*(60/(86400/COUNT(var13))))/1000 as `IT` 
FROM data
GROUP BY 1, 2;
INSERT INTO dataMonth (central, date, var1,var2,var3,var4,var5,var6,var7,var8,var9)   
SELECT DISTINCT central, CONCAT( DATE_FORMAT(from_unixtime(`date`), '%Y'), ":", DATE_FORMAT(from_unixtime(`date`), '%m'), ":", DATE_FORMAT(from_unixtime(`date`), '%d')) dateInYear, 
ROUND(AVG(var1),1) `Tension AC Media`,
ROUND((SUM(var3)/(60*(60/(86400/COUNT(var3)))))/1000,1) as `Energia AC generada`,
ROUND(AVG(var6),1) as `Frecuencia`, 
ROUND(SUM(var12)/(60*(60/(86400/COUNT(var12))))/1000,1) as `Energia DC generada`, 
ROUND(SUM(var13)/(60*(60/(86400/COUNT(var13))))/1000,1) as `Irradiacion total`, 
ROUND(AVG(var15),1) as `Temp. ambiente media`,
SUM(var3)/(60*(60/(86400/COUNT(var3))))/1000 as `EACG`, 
SUM(var12)/(60*(60/(86400/COUNT(var12))))/1000 as `EDCG`, 
SUM(var13)/(60*(60/(86400/COUNT(var13))))/1000 as `IT` 
FROM data
GROUP BY 1, 2;
END;$$ 
DELIMITER ;

delimiter //
CREATE TRIGGER ins_check BEFORE INSERT ON data FOR EACH ROW BEGIN 
IF NEW.var13 < 40 THEN
SET NEW.var10 = 0;
END IF; 
IF NEW.var13 < 70 THEN 
SET NEW.var2 = 0;
SET NEW.var3 = 0;
SET NEW.var4 = 0;
SET NEW.var5 = 0;
SET NEW.var11 = 0;
SET NEW.var12 = 0;
END IF;
END;//
delimiter ;