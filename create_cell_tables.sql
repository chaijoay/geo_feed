CREATE TABLE "CFMS"."PROVINCE_MASTER"
(
    "SHORTCODE"     VARCHAR2(3)     NOT NULL ENABLE,
    "PROV_NAME_TH"  VARCHAR2(100),
    "PROV_NAME_EN"  VARCHAR2(100),
    "RADIUS"        NUMBER(*,0)     NOT NULL ENABLE,
    "LAT1"          VARCHAR2(1)     NOT NULL ENABLE,
    "LAT2"          NUMBER(*,0)     NOT NULL ENABLE,
    "LAT3"          NUMBER(*,0)     NOT NULL ENABLE,
    "LAT4"          NUMBER(*,0)     NOT NULL ENABLE,
    "LNG1"          VARCHAR2(1)     NOT NULL ENABLE,
    "LNG2"          NUMBER(*,0)     NOT NULL ENABLE,
    "LNG3"          NUMBER(*,0)     NOT NULL ENABLE,
    "LNG4"          NUMBER(*,0)     NOT NULL ENABLE
) TABLESPACE "CUSTOM_USED";
CREATE UNIQUE INDEX "PROV_MST_IDX" ON "CFMS"."PROVINCE_MASTER" ("SHORTCODE")
    TABLESPACE "CUSTOM_USED_IDX";
CREATE UNIQUE INDEX "PROV_MST_IDX2" ON "CFMS"."PROVINCE_MASTER" ("PROV_NAME_TH")
    TABLESPACE "CUSTOM_USED_IDX";
CREATE UNIQUE INDEX "PROV_MST_IDX3" ON "CFMS"."PROVINCE_MASTER" ("PROV_NAME_EN")
    TABLESPACE "CUSTOM_USED_IDX";

CREATE TABLE "CFMS"."COUNTRY_MASTER"
(
    "MCC"           VARCHAR2(4)     NOT NULL ENABLE,
    "REGION_CODE"   VARCHAR2(3)     NOT NULL ENABLE,
    "COUNTRY_ABBR"  VARCHAR2(5)     NOT NULL ENABLE,
    "COUNTRY_NAME"  VARCHAR2(100)   NOT NULL ENABLE,
    "RADIUS"        NUMBER(*,0)     NOT NULL ENABLE,
    "LAT1"          VARCHAR2(1)     NOT NULL ENABLE,
    "LAT2"          NUMBER(*,0)     NOT NULL ENABLE,
    "LAT3"          NUMBER(*,0)     NOT NULL ENABLE,
    "LAT4"          NUMBER(*,0)     NOT NULL ENABLE,
    "LNG1"          VARCHAR2(1)     NOT NULL ENABLE,
    "LNG2"          NUMBER(*,0)     NOT NULL ENABLE,
    "LNG3"          NUMBER(*,0)     NOT NULL ENABLE,
    "LNG4"          NUMBER(*,0)     NOT NULL ENABLE
) TABLESPACE "CUSTOM_USED";
CREATE UNIQUE INDEX "CNTRY_MST_IDX" ON "CFMS"."COUNTRY_MASTER" ("MCC", "COUNTRY_ABBR", "COUNTRY_NAME")
    TABLESPACE "CUSTOM_USED_IDX";

CREATE SEQUENCE "CFMS"."SEQ_SWID"
    INCREMENT BY 1
    START WITH 0
    MINVALUE 0
    MAXVALUE 99999999
    NOCYCLE
    NOCACHE
    ORDER;

CREATE SEQUENCE "CFMS"."SEQ_CID"
    INCREMENT BY 1
    START WITH 0
    MINVALUE 0
    MAXVALUE 99999999
    NOCYCLE
    NOCACHE
    ORDER;

CREATE TABLE "CFMS"."CELL_AREA"
(
    "SID"           VARCHAR2(6) NOT NULL ENABLE,    -- TH+<ProvinceShortCode> or <Region> + <CountryShortCode>
    "DESC_TH"       VARCHAR2(100),                  -- Province Name or Country Name
    "DESC_EN"       VARCHAR2(100),                  -- Province Name or Country Name
    "LATITUDE"      VARCHAR2(10) NOT NULL ENABLE,   -- <Degree>-<Minute>-<Second><Direction(N,E)>
    "LONGITUDE"     VARCHAR2(10) NOT NULL ENABLE,   -- <Degree>-<Minute>-<Second><Direction(W,S)>
    "RADIUS"        NUMBER(*,0) NOT NULL ENABLE,
    "IS_UPDATED"    VARCHAR2(1) NOT NULL ENABLE,    -- Y to be fed to FRM otherwise skip
    "LAST_UPDATE"   DATE
) TABLESPACE "CUSTOM_USED";

CREATE TABLE "CFMS"."CELL_SET"
(
    "FRM_SWID"      VARCHAR2(7) NOT NULL ENABLE,    -- Running Number SEQ_SWID(DEC) To_Char
    "SID"           VARCHAR2(6) NOT NULL ENABLE,    -- TH+<ProvinceShortCode> or <Region> + <CountryShortCode>
    "SWITCH_NO"     VARCHAR2(100) NOT NULL ENABLE,  -- AMPHUR + TUMBOL
    "DESCRIPTION"   VARCHAR2(100),                  -- AMPHUR + TUMBOL
    "LATITUDE"      VARCHAR2(10) NOT NULL ENABLE,
    "LONGITUDE"     VARCHAR2(10) NOT NULL ENABLE,
    "RADIUS"        NUMBER(*,0) NOT NULL ENABLE,
    "IS_UPDATED"    VARCHAR2(10) NOT NULL ENABLE,   -- Y to be fed to FRM otherwise skip
    "LAST_UPDATE"   DATE
) TABLESPACE "CUSTOM_USED";

CREATE TABLE "CFMS"."CELL"
(
    "FRM_CELL_ID"   VARCHAR2(5) NOT NULL ENABLE,    -- Running Number SEQ_CID(DEC) To_Char (to_Hex)
    "SID"           VARCHAR2(6) NOT NULL ENABLE,    -- TH+<ProvinceShortCode> or <Region> + <CountryShortCode>
    "SWITCH_NO"     VARCHAR2(100) NOT NULL ENABLE,  -- AMPHUR + TUMBOL
    "CELL_ID"       VARCHAR2(25) NOT NULL ENABLE,   -- CELL CODE
    "DESCRIPTION"   VARCHAR2(100),
    "LATITUDE"      VARCHAR2(10) NOT NULL ENABLE,
    "LONGITUDE"     VARCHAR2(10) NOT NULL ENABLE,
    "RADIUS"        NUMBER(*,0) NOT NULL ENABLE,
    "CGI"           VARCHAR2(30) NOT NULL ENABLE,
    "IS_UPDATED"    VARCHAR2(1) NOT NULL ENABLE,    -- Y to be fed to FRM otherwise skip
    "LAST_UPDATE"   DATE
) TABLESPACE "CUSTOM_USED";

CREATE UNIQUE INDEX "CELL_AREA_IDX" ON "CFMS"."CELL_AREA" ("SID")
    TABLESPACE "CUSTOM_USED_IDX";
CREATE UNIQUE INDEX "CELL_AREA_IDX2" ON "CFMS"."CELL_AREA" ("DESC_TH")
    TABLESPACE "CUSTOM_USED_IDX";
CREATE UNIQUE INDEX "CELL_AREA_IDX3" ON "CFMS"."CELL_AREA" ("DESC_EN")
    TABLESPACE "CUSTOM_USED_IDX";

CREATE UNIQUE INDEX "CELL_SET_IDX" ON "CFMS"."CELL_SET" ("SID", "SWITCH_NO")
    TABLESPACE "CUSTOM_USED_IDX";
CREATE UNIQUE INDEX "CELL_SET_IDX2" ON "CFMS"."CELL_SET" ("FRM_SWID")
    TABLESPACE "CUSTOM_USED_IDX";

CREATE UNIQUE INDEX "CELL_IDX" ON "CFMS"."CELL" ("SID", "SWITCH_NO", "CELL_ID")
    TABLESPACE "CUSTOM_USED_IDX";
CREATE UNIQUE INDEX "CELL_IDX2" ON "CFMS"."CELL" ("FRM_CELL_ID")
    TABLESPACE "CUSTOM_USED_IDX";
CREATE INDEX "CELL_IDX3" ON "CFMS"."CELL" ("CGI")
    TABLESPACE "CUSTOM_USED_IDX";


gawk -F "|" '{ if (toupper($22) ~ /^(ACTIVE|ACTIVATED|UNLOCKED|1)$/) { print $0; } }' file | sort -T "|" -k 17,17 -k 18,18 -k 19,19, -k 20,20

--- CELL_AREA ---
MERGE INTO CFMS.CELL_AREA tgt USING (SELECT XXXX FROM DUAL) src
        ON (tgt.SID = src.SID)
    WHEN MATCHED
        THEN UPDATE SET
            tgt.DESCRIPTION = src.DESCRIPTION,
            tgt.LATITUDE = src.LATITUDE,
            tgt.LONGITUDE = src.LONGITUDE,
            tgt.RADIUS = src.RADIUS,
            tgt.IS_UPDATED = 'Y',
            tgt.LAST_UPDATE = SYSDATE
        WHERE
            (tgt.DESCRIPTION <> src.DESCRIPTION)
            OR (tgt.LATITUDE <> src.LATITUDE)
            OR (tgt.LONGITUDE<> src.LONGITUDE)
            OR (tgt.RADIUS<> src.RADIUS)
    WHEN NOT MATCHED
        THEN INSERT (tgt.SID, tgt.DESCRIPTION, tgt.LATITUDE, tgt.LONGITUDE, tgt.RADIUS, tgt.IS_UPDATED, tgt.LAST_UPDATE)
            VALUES (src.SID, src.DESCRIPTION, src.LATITUDE, src.LONGITUDE, src.RADIUS, 'Y', SYSDATE);

--- CELL_SET ---
MERGE INTO CFMS.CELL_SET tgt USING (SELECT XXXX FROM DUAL) src
        ON (tgt.SID = src.SID AND tgt.SWITCH_NO = src.SWITCH_NO)
    WHEN MATCHED
        THEN UPDATE SET
            tgt.DESCRIPTION = src.DESCRIPTION,
            tgt.LATITUDE = src.LATITUDE,
            tgt.LONGITUDE = src.LONGITUDE,
            tgt.RADIUS = src.RADIUS,
            tgt.IS_UPDATED = 'Y',
            tgt.LAST_UPDATE = SYSDATE
        WHERE
            (tgt.DESCRIPTION <> src.DESCRIPTION)
            OR (tgt.LATITUDE <> src.LATITUDE)
            OR (tgt.LONGITUDE <> src.LONGITUDE)
            OR (tgt.RADIUS <> src.RADIUS)
    WHEN NOT MATCHED
        THEN INSERT (tgt.FRM_SWID, tgt.SID, tgt.SWITCH_NO, tgt.DESCRIPTION, tgt.LATITUDE, tgt.LONGITUDE, tgt.RADIUS, tgt.IS_UPDATED, tgt.LAST_UPDATE)
            VALUES (TO_CHAR(SEQ_SWID.NEXTVAL,'0000000'), src.SID, src.SWITCH_NO, src.DESCRIPTION, src.LATITUDE, src.LONGITUDE, src.RADIUS, 'Y', SYSDATE);

--- CELL ---
MERGE INTO CFMS.CELL tgt USING (SELECT XXXX FROM DUAL) src
        ON (tgt.SID=src.SID AND tgt.SWITCH_NO = src.SWITCH_NO AND tgt.CELL_ID = src.CELL_ID)
    WHEN MATCHED
        THEN UPDATE SET
            tgt.DESCRIPTION = src.DESCRIPTION,
            tgt.LATITUDE = src.LATITUDE,
            tgt.LONGITUDE = src.LONGITUDE,
            tgt.RADIUS = src.RADIUS,
            tgt.CGI = src.CGI,
            tgt.IS_UPDATED = 'Y'
            tgt.LAST_UPDATE = SYSDATE
        WHERE
            (tgt.DESCRIPTION <> src.DESCRIPTION)
            OR (tgt.LATITUDE <> src.LATITUDE)
            OR (tgt.LONGITUDE <> src.LONGITUDE)
            OR (tgt.RADIUS <> src.RADIUS)
            OR (tgt.CGI <> src.CGI)
    WHEN NOT MATCHED
        THEN INSERT (tgt.FRM_CELL_ID, tgt.SID, tgt.SWITCH_NO, tgt.CELL_ID, tgt.DESCRIPTION, tgt.LATITUDE, tgt.LONGITUDE, tgt.CGI, tgt.RADIUS, tgt.IS_UPDATED, tgt.LAST_UPDATE)
            VALUES (TO_CHAR(SEQ_CID.NEXTVAL,'0XXXXX'), src.SID, src.SWITCH_NO, src.CELL_ID, src.DESCRIPTION, src.LATITUDE, src.CGI, src.LONGITUDE, src.RADIUS, 'Y', SYSDATE);

select sid, description, latitude, longitude, radius, rowid from cell_area where is_update = 'Y';           -- S%-2
select sid, frm_swid, description, latitude, longitude, radius, rowid from cell_set where is_update = 'Y';  -- W%-2
select c.sid, s.frm_swid, c.frm_cell_id, c.description, c.latitude, c.longitude, c.radius, c.rowid from cell c, cell_set s
where c.is_update = 'Y' and c.sid = s.sid and c.switch_no = s.switch_no;                                    -- C%-2



