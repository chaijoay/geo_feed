///
///
/// FACILITY    : Maintaining of Cell Geo-information for CDR Feed Mapping of FRM geographic information
///
/// FILE NAME   : geo_feed.h
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
#ifndef __GEO_FEED_H__
#define __GEO_FEED_H__

#ifdef  __cplusplus
    extern "C" {
#endif

#define _APP_NAME_          "geo_feed"
#define _APP_VERS_          "1.0"


// ----- INI Parameters -----
// All Section
typedef enum {
    E_INPUT = 0,
    E_OUTPUT,
    E_COMMON,
    E_DBCONN_ERM,
    E_NOF_SECTION
} E_INI_SECTION;

typedef enum {
    // OUTPUT Section
    E_OUT_CDRFEED_DIR = 0,
    E_OUT_DIR,
    E_OUT_PRE_SID,
    E_OUT_PRE_SWID,
    E_OUT_PRE_CELL,
    E_OUT_SUFF,
    E_DEF_RADIUS,
    E_NOF_PAR_OUTPUT
} E_INI_OUTPUT_SEC;

typedef enum {
    // INPUT Section
    E_INP_DIR = 0,
    E_INP_CONTAIN,
    E_INP_SUFFIX,
    E_NOF_PAR_INPUT
} E_INI_INPUT_SEC;

// typedef enum {
//     E_MAPPING_FILE = 0,
//     E_NOF_PAR_MAP
// } E_INI_MAP_SEC;

typedef enum {
    // COMMON Section
    E_TMP_DIR,
    E_BCK_INP,
    E_BCK_INP_DIR,
    E_BCK_OUT,
    E_BCK_OUT_DIR,
    E_LOG_DIR,
    E_LOG_LEVEL,
    E_REC_COMMIT,
    E_STATE_DIR,
    E_STATE_DAY,
    E_ENB_ERR_FILE,
    E_IGN_DIR,
    E_ERR_DIR,
    E_NOF_PAR_COMMON
} E_INI_COMMON_SEC;


typedef enum {
    // DB Section
    E_DBS_USER = 0,
    E_DBS_PASSWORD,
    E_DBS_DB_SID,
    E_NOF_PAR_DBCONN
} E_INI_DBCONN_DBS_SEC;


int     readConfig(int argc, char *argv[]);
void    logHeader();
void    printUsage();
void    clearCksum(char *file);
int     validateIni();
int     _ini_callback(const char *section, const char *key, const char *value, void *userdata);
void    makeIni();


#ifdef  __cplusplus
    }
#endif


#endif  // __GEO_FEED_H__

