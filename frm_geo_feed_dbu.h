///
///
/// FACILITY    : db utility for
///
/// FILE NAME   : geo_feed_dbu.h
///
/// AUTHOR      : Thanakorn Nitipiromchai
///
/// CREATE DATE : 02-Mar-2020
///
/// CURRENT VERSION NO : 1.0
///
/// LAST RELEASE DATE  : 02-Mar-2020
///
/// MODIFICATION HISTORY :
///     1.0     02-Mar-2020     First Version
///
///
#ifndef __GEO_FEED_DBU_H__
#define __GEO_FEED_DBU_H__

#ifdef  __cplusplus
    extern "C" {
#endif

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>

// #include <sqlca.h>
// #include <sqlda.h>
// #include <sqlcpr.h>

#include "frm_geo_feed.h"
#include "strlogutl.h"
#include "glb_str_def.h"

#define NOT_FOUND               1403
#define KEY_DUP                 -1
#define DEADLOCK                -60
#define FETCH_OUTOFSEQ          -1002
#define MAX_REC_PER_FILE        50000

#define SIZE_GEN_STR            100
#define SIZE_SQL_STR            1024

#define FLG_FOR_LOAD            'Y'
#define FLG_CMPL_FED            'N'

#define ALL_PARTITION           "-2"
#define EFIT_DELIM              '#'
#define FLG_EFIT_SID            'S'
#define FLG_EFIT_SWID           'W'
#define FLG_EFIT_CELL           'C'

#define NUM_ERR_ALLOW           150000

#define SUFF_IGN                ".IGN"
#define SUFF_ERR                ".ERR"

#define IS_IGN                  1
#define IS_ERR                  2

#define ERM_SID                 6+1
#define ERM_SWID                7+1
#define ERM_CELL                5+1
#define ERM_DESC                100+1

#define USR_SID                 ERM_SID
#define USR_SWID                100+1
#define USR_CELL                30+1

#define USR_LATLNG              15

#define _CELL_ID_               0
#define _CELL_SET_              1

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum {
    E_SID = 0,
    E_DESC,
    E_LAT,
    E_LONG,
    E_RADIUS,
    E_NOF_CELL_AREA
} E_FLD_CELL_AREA;

typedef enum {
    E_REGION,
    E_LOC_NAME,
    E_SITE_NAME,    // use this column
    E_SITE_CODE,
    E_LAT1,         // use this column  (Direction N/S)
    E_LAT2,         // use this column  (Degree 0-89)
    E_LAT3,         // use this column  (Minute 0-59)
    E_LAT4,         // use this column  (Second 0-59)
    E_LONG1,        // use this column  (Direction E/W)
    E_LONG2,        // use this column  (Degree 0-179)
    E_LONG3,        // use this column  (Minutes 0-59)
    E_LONG4,        // use this column  (Seconde 0-59)
    E_WGS72_LAT,
    E_WGS72_LONG,
    E_WGS84_LAT,
    E_WGS84_LONG,
    E_PROVINCE,     // use this column
    E_AMPHUR,       // use this column
    E_TUMBOL,       // use this column
    E_CELL_CODE,    // use this column
    E_CGI,          // use this column
    E_CELL_STAT,    // use this column
    E_NOF_CELL_ID
} E_FLD_CELL_ID;



// === common function ===
void    setCommitRec(int commit_cnt);
void    setDefRadius(int def_radius);
int     connectDbErm(char *szDbUsr, char *szDbPwd, char *szDbSvr);
void    disconnErm();

int     getCurrId(int id_type);
void    setCurrId(int id_type, int currval);

int     updateProcRecord(const char *tab_name, const char *row_id, char feed_flg);

void    procInsUpd(char inp_ini[][SIZE_ITEM_L], char comm_ini[][SIZE_ITEM_L]);
void    insUpdCellAreaTH();
void    insUpdCellAreaEN();
void    insUpdCellSetAndCell(char inp_ini[][SIZE_ITEM_L], char comm_ini[][SIZE_ITEM_L]);
int     insUpdCellSet(char *errstr);
int     insUpdCell(const char *cell_type, char *errstr);

int     validateGeoField(int err_or_ign, char *info);

void    procWrtOuput(char out_ini[][SIZE_ITEM_L], char comm_ini[][SIZE_ITEM_L]);
void    wrtCellArea(char out_ini[][SIZE_ITEM_L], char comm_ini[][SIZE_ITEM_L]);
void    wrtCellSet(char out_ini[][SIZE_ITEM_L], char comm_ini[][SIZE_ITEM_L]);
void    wrtCell(char out_ini[][SIZE_ITEM_L], char comm_ini[][SIZE_ITEM_L]);

void    wrtErrInfo(const char *fname, const char *rec_inf, int wrt);
void    wrtIgnRecord(const char *fname, const char *rec_inf, int wrt);

#if 0
int     cksum_pass(const char *file);
#endif

#ifdef  __cplusplus
    }
#endif

#endif
