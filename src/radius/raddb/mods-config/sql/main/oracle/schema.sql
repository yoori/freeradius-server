--
-- $Id$
--
-- Oracle schema for FreeRADIUS
--
--
-- NOTE: Which columns are NULLable?
--       Oracle has a non-standard "feature" in that it treats an empty string a NULL!
--

--
-- Table structure for table 'radacct'
--
CREATE TABLE radacct (
	radacctid		INT PRIMARY KEY,
	acctsessionid		VARCHAR(96) NOT NULL,
	acctuniqueid		VARCHAR(32),
	username		VARCHAR(64),
	groupname		VARCHAR(32),
	realm			VARCHAR(30),
	nasipaddress		VARCHAR(15),
	nasportid		VARCHAR(32),
	nasporttype		VARCHAR(32),
	acctstarttime		TIMESTAMP WITH TIME ZONE,
	acctstoptime		TIMESTAMP WITH TIME ZONE,
	acctsessiontime		NUMERIC(19),
	acctauthentic		VARCHAR(32),
	connectinfo_start	VARCHAR(50),
	connectinfo_stop	VARCHAR(50),
	acctinputoctets		NUMERIC(19),
	acctoutputoctets	NUMERIC(19),
	calledstationid		VARCHAR(50),
	callingstationid	VARCHAR(50),
	acctterminatecause	VARCHAR(32),
	servicetype		VARCHAR(32),
	framedprotocol		VARCHAR(32),
	framedipaddress		VARCHAR(15),
	framedipv6address	VARCHAR(45),
	framedipv6prefix	VARCHAR(45),
	framedinterfaceid	VARCHAR(44),
	delegatedipv6prefix	VARCHAR(45),
	acctstartdelay		NUMERIC(12),
	class 			VARCHAR(64)
);

CREATE UNIQUE INDEX radacct_idx0
	ON radacct(acctuniqueid);
CREATE UNIQUE INDEX radacct_idx1
	ON radacct(acctsessionid,username,acctstarttime,
		acctstoptime,nasipaddress,framedipaddress,framedipv6address,framedipv6prefix,framedinterfaceid,delegatedipv6prefix);
CREATE INDEX radacct_idx2
        ON radacct(acctstoptime,nasipaddress,acctstarttime);

CREATE SEQUENCE radacct_seq START WITH 1 INCREMENT BY 1;

--
-- Trigger to emulate a serial # on the primary key
--
CREATE OR REPLACE TRIGGER radacct_serialnumber
	BEFORE INSERT OR UPDATE OF radacctid ON radacct
	FOR EACH ROW
	BEGIN
		if ( :new.radacctid = 0 or :new.radacctid is null ) then
			SELECT radacct_seq.nextval into :new.radacctid from dual;
		end if;
	END;
/

--
-- Table structure for table 'radcheck'
--
CREATE TABLE radcheck (
	id 		INT PRIMARY KEY,
	username	VARCHAR(30) NOT NULL,
	attribute	VARCHAR(64),
	op		VARCHAR(2) NOT NULL,
	value		VARCHAR(40)
);
CREATE SEQUENCE radcheck_seq START WITH 1 INCREMENT BY 1;

-- Trigger to emulate a serial # on the primary key
CREATE OR REPLACE TRIGGER radcheck_serialnumber
	BEFORE INSERT OR UPDATE OF id ON radcheck
	FOR EACH ROW
	BEGIN
		if ( :new.id = 0 or :new.id is null ) then
			SELECT radcheck_seq.nextval into :new.id from dual;
		end if;
	END;
/

--
-- Table structure for table 'radgroupcheck'
--
CREATE TABLE radgroupcheck (
	id 		INT PRIMARY KEY,
	groupname	VARCHAR(20) UNIQUE NOT NULL,
	attribute	VARCHAR(64),
	op		CHAR(2) NOT NULL,
	value		VARCHAR(40)
);
CREATE SEQUENCE radgroupcheck_seq START WITH 1 INCREMENT BY 1;

-- Trigger to emulate a serial # on the primary key
CREATE OR REPLACE TRIGGER radgroupcheck_serialnumber
	BEFORE INSERT OR UPDATE OF id ON radgroupcheck
	FOR EACH ROW
	BEGIN
		if ( :new.id = 0 or :new.id is null ) then
			SELECT radgroupcheck_seq.nextval into :new.id from dual;
		end if;
	END;
/

--
-- Table structure for table 'radgroupreply'
--
CREATE TABLE radgroupreply (
	id		INT PRIMARY KEY,
	GroupName	VARCHAR(20) UNIQUE NOT NULL,
	Attribute	VARCHAR(64),
	op		CHAR(2) NOT NULL,
	Value		VARCHAR(40)
);
CREATE SEQUENCE radgroupreply_seq START WITH 1 INCREMENT BY 1;

-- Trigger to emulate a serial # on the primary key
CREATE OR REPLACE TRIGGER radgroupreply_serialnumber
	BEFORE INSERT OR UPDATE OF id ON radgroupreply
	FOR EACH ROW
	BEGIN
		if ( :new.id = 0 or :new.id is null ) then
			SELECT radgroupreply_seq.nextval into :new.id from dual;
		end if;
	END;
/

--
-- Table structure for table 'radreply'
--
CREATE TABLE radreply (
	id		INT PRIMARY KEY,
	UserName	VARCHAR(30) NOT NULL,
	Attribute	VARCHAR(64),
	op		CHAR(2) NOT NULL,
	Value		VARCHAR(40)
);
CREATE INDEX radreply_idx1 ON radreply(UserName);
CREATE SEQUENCE radreply_seq START WITH 1 INCREMENT BY 1;

--
-- Trigger to emulate a serial # on the primary key
--
CREATE OR REPLACE TRIGGER radreply_serialnumber
	BEFORE INSERT OR UPDATE OF id ON radreply
	FOR EACH ROW
	BEGIN
		if ( :new.id = 0 or :new.id is null ) then
			SELECT radreply_seq.nextval into :new.id from dual;
		end if;
	END;
/

--
-- Table structure for table 'radusergroup'
--
CREATE TABLE radusergroup (
	id		INT PRIMARY KEY,
	UserName	VARCHAR(30) NOT NULL,
	GroupName	VARCHAR(30),
	Priority	INT
);
CREATE INDEX radusergroup_idx1 ON radusergroup(UserName);
CREATE SEQUENCE radusergroup_seq START WITH 1 INCREMENT BY 1;

--
-- Trigger to emulate a serial # on the primary key
--
CREATE OR REPLACE TRIGGER radusergroup_serialnumber
	BEFORE INSERT OR UPDATE OF id ON radusergroup
	FOR EACH ROW
	BEGIN
		if ( :new.id = 0 or :new.id is null ) then
			SELECT radusergroup_seq.nextval into :new.id from dual;
		end if;
	END;
/


CREATE TABLE radpostauth (
	  id            INT PRIMARY KEY,
	  UserName      VARCHAR(64) NOT NULL,
	  Pass          VARCHAR(64),
	  Reply         VARCHAR(64),
	  AuthDate 	TIMESTAMP(6) WITH TIME ZONE,
	  Class         VARCHAR(64)
);

CREATE SEQUENCE radpostauth_seq START WITH 1 INCREMENT BY 1;

CREATE OR REPLACE TRIGGER radpostauth_TRIG
	BEFORE INSERT OR UPDATE OF id ON radpostauth
	FOR EACH ROW
	BEGIN
		if ( :new.id = 0 or :new.id is null ) then
			SELECT radpostauth_seq.nextval into :new.id from dual;
		end if;
		if (:new.AuthDate is null) then
		  select systimestamp into :new.AuthDate from dual;
		end if;
	END;

/

--
-- Table structure for table 'nas'
--
CREATE TABLE nas (
	id              INT PRIMARY KEY,
	nasname         VARCHAR(128),
	shortname       VARCHAR(32),
	type            VARCHAR(30),
	ports           INT,
	secret          VARCHAR(60),
	server          VARCHAR(64),
	community       VARCHAR(50),
	description     VARCHAR(200)
);
CREATE SEQUENCE nas_seq START WITH 1 INCREMENT BY 1;

-- Trigger to emulate a serial # on the primary key
CREATE OR REPLACE TRIGGER nas_serialnumber
	BEFORE INSERT OR UPDATE OF id ON nas
	FOR EACH ROW
	BEGIN
		if ( :new.id = 0 or :new.id is null ) then
			SELECT nas_seq.nextval into :new.id from dual;
		end if;
	END;
/

