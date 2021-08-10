///
///
/// FACILITY    : Maintaining of Cell Geo-information for CDR Feed Mapping of FRM geographic information
///
/// FILE NAME   : geo_feed.c
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

#include <time.h>
#include <errno.h>
#include <stdio.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

#include "minIni.h"
#include "procsig.h"
#include "geo_feed.h"
#include "strlogutl.h"
#include "geo_feed_dbu.h"
#include "snapfile.h"

#define MODE_INSUPD_DB  0
#define MODE_WRT_OUPUT  1
#define MODE_FULL_PROC  2

char gszAppName[SIZE_ITEM_S];
char gszIniFile[SIZE_FULL_NAME];
int  gnMode;

const char gszIniStrSection[E_NOF_SECTION][SIZE_ITEM_T] = {
    "INPUT",
    "OUTPUT",
    "COMMON",
    "DB_CONNECTION_ERM"
};

const char gszIniStrOutput[E_NOF_PAR_OUTPUT][SIZE_ITEM_T] = {
    "OUT_CDRFEED_DIR",
    "OUTPUT_DIR",
    "OUT_SID_PREFIX",
    "OUT_SWID_PREFIX",
    "OUT_CELL_PREFIX",
    "OUT_FILE_SUFFIX",
    "DEF_CELL_RADIUS"
};

const char gszIniStrInput[E_NOF_PAR_INPUT][SIZE_ITEM_T] = {
    "INPUT_DIR",
    "INP_NAME_CONTAIN",
    "INP_FILE_SUFFIX"
};

const char gszIniStrCommon[E_NOF_PAR_COMMON][SIZE_ITEM_T] = {
    "TMP_DIR",
    "BCK_INP",
    "BCK_INP_DIR",
    "BCK_OUT",
    "BCK_OUT_DIR",
    "LOG_DIR",
    "LOG_LEVEL",
    "REC_COMMIT",
    "STATE_DIR",
    "KEEP_STATE_DAY",
    "ENABLE_ERR_FILE",
    "IGN_DIR",
    "ERR_DIR"
};

const char gszIniStrDbConnErm[E_NOF_PAR_DBCONN][SIZE_ITEM_T] = {
    "ERM_USER_NAME",
    "ERM_PASSWORD",
    "ERM_DB_SID"
};

char gszIniParOutput[E_NOF_PAR_OUTPUT][SIZE_ITEM_L];
char gszIniParInput[E_NOF_PAR_INPUT][SIZE_ITEM_L];
// char gszIniParMapping[E_NOF_PAR_MAP][SIZE_ITEM_L];
char gszIniParCommon[E_NOF_PAR_COMMON][SIZE_ITEM_L];
char gszIniParDbConnErm[E_NOF_PAR_DBCONN][SIZE_ITEM_L];

int main(int argc, char *argv[])
{

    memset(gszAppName, 0x00, sizeof(gszAppName));
    memset(gszIniFile, 0x00, sizeof(gszIniFile));

    // 1. read ini file
    gnMode = MODE_FULL_PROC;
    if ( readConfig(argc, argv) != SUCCESS ) {
        return EXIT_FAILURE;
    }

    if ( procLock(gszAppName, E_CHK) != SUCCESS ) {
        fprintf(stderr, "another instance of %s is running\n", gszAppName);
        return EXIT_FAILURE;
    }

    if ( handleSignal() != SUCCESS ) {
        fprintf(stderr, "init handle signal failed: %s\n", getSigInfoStr());
        return EXIT_FAILURE;
    }

    if ( startLogging(gszIniParCommon[E_LOG_DIR], gszAppName, atoi(gszIniParCommon[E_LOG_LEVEL])) != SUCCESS ) {
       return EXIT_FAILURE;
    }

    if ( validateIni() == FAILED ) {
        return EXIT_FAILURE;
    }
    logHeader();

    procLock(gszAppName, E_SET);

    if ( connectDbErm(gszIniParDbConnErm[E_DBS_USER], gszIniParDbConnErm[E_DBS_PASSWORD], gszIniParDbConnErm[E_DBS_DB_SID]) != SUCCESS ) {
        procLock(gszAppName, E_CLR);
        stopLogging();
        return EXIT_FAILURE;
    }

#if 0
    if ( gnClear == 1 ) {
        clearCksum(gszIniParMapping[E_MAPPING_FILE]);
    }
#endif

    if ( startState(gszIniParCommon[E_STATE_DIR], gszAppName, atoi(gszIniParCommon[E_STATE_DAY])) != SUCCESS ) {
        procLock(gszAppName, E_CLR);
        stopLogging();
        return EXIT_FAILURE;
    }

    setCommitRec(atoi(gszIniParCommon[E_REC_COMMIT]));
    setDefRadius(atoi(gszIniParOutput[E_DEF_RADIUS]));
    if ( gnMode == MODE_INSUPD_DB ) {
        writeLog(LOG_INF, "starting with Insert/Update Mode");
        procInsUpd(gszIniParInput, gszIniParCommon);
    }
    else if ( gnMode == MODE_WRT_OUPUT ) {
        writeLog(LOG_INF, "starting with write output Mode");
        procWrtOuput(gszIniParOutput, gszIniParCommon);
    }
    else {
        writeLog(LOG_INF, "starting with full process Mode");
        procInsUpd(gszIniParInput, gszIniParCommon);
        procWrtOuput(gszIniParOutput, gszIniParCommon);
    }

    disconnErm();
    writeLog(LOG_INF, "------- %s process completely stop -------", _APP_NAME_);

    procLock(gszAppName, E_CLR);
    stopLogging();

    return EXIT_SUCCESS;

}

int readConfig(int argc, char *argv[])
{

    char appPath[SIZE_ITEM_L];
    int key, i;

    memset(gszIniFile, 0x00, sizeof(gszIniFile));
    memset(gszAppName, 0x00, sizeof(gszAppName));

    memset(gszIniParOutput, 0x00, sizeof(gszIniParOutput));
    memset(gszIniParInput, 0x00, sizeof(gszIniParInput));
    // memset(gszIniParMapping, 0x00, sizeof(gszIniParMapping));
    memset(gszIniParCommon, 0x00, sizeof(gszIniParCommon));
    memset(gszIniParDbConnErm, 0x00, sizeof(gszIniParDbConnErm));

    strcpy(appPath, argv[0]);
    char *p = strrchr(appPath, '/');
    *p = '\0';

    for ( i = 1; i < argc; i++ ) {
        if ( strcmp(argv[i], "-n") == 0 ) {     // specified ini file
            strcpy(gszIniFile, argv[++i]);
        }
        else if ( strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 ) {
            printUsage();
            return FAILED;
        }
        else if ( strcmp(argv[i], "-mkini") == 0 ) {
            makeIni();
            return FAILED;
        }
        else if ( strcmp(argv[i], "-upd") == 0 ) {
            gnMode = MODE_INSUPD_DB;
        }
        else if ( strcmp(argv[i], "-wrt") == 0 ) {
            gnMode = MODE_WRT_OUPUT;
        }
        else if ( strcmp(argv[i], "-all") == 0 ) {
            gnMode = MODE_FULL_PROC;
        }
        else {
            fprintf(stderr, "unknow '%s' parameter\n", argv[i]);
            printUsage();
            return FAILED;
        }
    }

    if ( gszIniFile[0] == '\0' ) {
        sprintf(gszIniFile, "%s/%s.ini", appPath, _APP_NAME_);
    }
    sprintf(gszAppName, "%s", _APP_NAME_);

    if ( access(gszIniFile, F_OK|R_OK) != SUCCESS ) {
        fprintf(stderr, "unable to access ini file %s (%s)\n", gszIniFile, strerror(errno));
        return FAILED;
    }

    // Read config of OUTPUT Section
    for ( key = 0; key < E_NOF_PAR_OUTPUT; key++ ) {
        ini_gets(gszIniStrSection[E_OUTPUT], gszIniStrOutput[key], "NA", gszIniParOutput[key], sizeof(gszIniParOutput[key]), gszIniFile);
    }

    // Read config of INPUT Section
    for ( key = 0; key < E_NOF_PAR_INPUT; key++ ) {
        ini_gets(gszIniStrSection[E_INPUT], gszIniStrInput[key], "NA", gszIniParInput[key], sizeof(gszIniParInput[key]), gszIniFile);
    }

    // Read config of COMMON Section
    for ( key = 0; key < E_NOF_PAR_COMMON; key++ ) {
        ini_gets(gszIniStrSection[E_COMMON], gszIniStrCommon[key], "NA", gszIniParCommon[key], sizeof(gszIniParCommon[key]), gszIniFile);
    }

    // Read config of ERM DB Connection Section
    for ( key = 0; key < E_NOF_PAR_DBCONN; key++ ) {
        ini_gets(gszIniStrSection[E_DBCONN_ERM], gszIniStrDbConnErm[key], "NA", gszIniParDbConnErm[key], sizeof(gszIniParDbConnErm[key]), gszIniFile);
    }

    return SUCCESS;

}

void logHeader()
{
    writeLog(LOG_INF, "---- Start %s (v%s) with following parameters ----", _APP_NAME_, _APP_VERS_);
    // print out all ini file
    ini_browse(_ini_callback, NULL, gszIniFile);
}

void printUsage()
{

    fprintf(stderr, "\nusage: %s version %s\n", _APP_NAME_, _APP_VERS_);

    fprintf(stderr, "\tMaintaining of Cells information for CDR Feed Mapping\n");
    fprintf(stderr, "\tto be used by HPE-FRM geography information.\n\n");

    fprintf(stderr, "%s.exe <-upd|-wrt|-all> [-n <ini_file>] [-mkini]\n", _APP_NAME_);
    fprintf(stderr, "\t-upd\tto perform maintainance(insert/update) of geo info in external db tables\n");
    fprintf(stderr, "\t-wrt\tto perform write output for efit and cdr feed\n");
    fprintf(stderr, "\t-all\tto perform all maintainance and write output\n");

    fprintf(stderr, "\tini_file\tto specify ini file other than default ini\n");
    fprintf(stderr, "\t-mkini\t\tto create ini template\n");
    fprintf(stderr, "\n");


}

int validateIni()
{
    int result = SUCCESS;

    // ----- Output Section -----
    if ( access(gszIniParOutput[E_OUT_DIR], W_OK|R_OK) != SUCCESS ) {
        result = FAILED;
        fprintf(stderr, "unable to access %s %s (%s)\n", gszIniStrOutput[E_OUT_DIR], gszIniParOutput[E_OUT_DIR], strerror(errno));
    }

    if ( access(gszIniParOutput[E_OUT_CDRFEED_DIR], W_OK|R_OK) != SUCCESS ) {
        result = FAILED;
        fprintf(stderr, "unable to access %s %s (%s)\n", gszIniStrOutput[E_OUT_CDRFEED_DIR], gszIniParOutput[E_OUT_CDRFEED_DIR], strerror(errno));
    }

    // ----- Common Section -----
    if ( access(gszIniParCommon[E_TMP_DIR], W_OK|R_OK) != SUCCESS ) {
        result = FAILED;
        fprintf(stderr, "unable to access %s %s (%s)\n", gszIniStrCommon[E_TMP_DIR], gszIniParCommon[E_TMP_DIR], strerror(errno));
    }
    if ( *gszIniParCommon[E_BCK_INP] == 'Y' || *gszIniParCommon[E_BCK_INP] == 'y' ) {
        strcpy(gszIniParCommon[E_BCK_INP], "Y");
        if ( access(gszIniParCommon[E_BCK_INP_DIR], W_OK|R_OK) != SUCCESS ) {
            result = FAILED;
            fprintf(stderr, "unable to access %s %s (%s)\n", gszIniStrCommon[E_BCK_INP_DIR], gszIniParCommon[E_BCK_INP_DIR], strerror(errno));
        }
    }
    if ( *gszIniParCommon[E_BCK_OUT] == 'Y' || *gszIniParCommon[E_BCK_OUT] == 'y' ) {
        strcpy(gszIniParCommon[E_BCK_OUT], "Y");
        if ( access(gszIniParCommon[E_BCK_OUT_DIR], F_OK|R_OK) != SUCCESS ) {
            result = FAILED;
            fprintf(stderr, "unable to access %s %s (%s)\n", gszIniStrCommon[E_BCK_OUT_DIR], gszIniParCommon[E_BCK_OUT_DIR], strerror(errno));
        }
    }
    if ( access(gszIniParCommon[E_LOG_DIR], W_OK|R_OK) != SUCCESS ) {
        result = FAILED;
        fprintf(stderr, "unable to access %s %s (%s)\n", gszIniStrCommon[E_LOG_DIR], gszIniParCommon[E_LOG_DIR], strerror(errno));
    }
    if ( access(gszIniParCommon[E_STATE_DIR], W_OK|R_OK) != SUCCESS ) {
        result = FAILED;
        fprintf(stderr, "unable to access %s %s (%s)\n", gszIniStrCommon[E_STATE_DIR], gszIniParCommon[E_STATE_DIR], strerror(errno));
    }

    // ----- Db Connection Section -----
    if ( *gszIniParDbConnErm[E_DBS_USER] == '\0' || strcmp(gszIniParDbConnErm[E_DBS_USER], "NA") == 0 ) {
        result = FAILED;
        fprintf(stderr, "invalid %s '%s'\n", gszIniStrDbConnErm[E_DBS_USER], gszIniParDbConnErm[E_DBS_USER]);
    }
    if ( *gszIniParDbConnErm[E_DBS_PASSWORD] == '\0' || strcmp(gszIniParDbConnErm[E_DBS_PASSWORD], "NA") == 0 ) {
        result = FAILED;
        fprintf(stderr, "invalid %s '%s'\n", gszIniStrDbConnErm[E_DBS_PASSWORD], gszIniParDbConnErm[E_DBS_PASSWORD]);
    }
    if ( *gszIniParDbConnErm[E_DBS_DB_SID] == '\0' || strcmp(gszIniParDbConnErm[E_DBS_DB_SID], "NA") == 0 ) {
        result = FAILED;
        fprintf(stderr, "invalid %s '%s'\n", gszIniStrDbConnErm[E_DBS_DB_SID], gszIniParDbConnErm[E_DBS_DB_SID]);
    }

    return result;

}

int _ini_callback(const char *section, const char *key, const char *value, void *userdata)
{
    if ( strstr(key, "PASSWORD") != NULL ) {
        writeLog(LOG_INF, "[%s]\t%s = %s", section, key, "********");
    }
    else {
        writeLog(LOG_INF, "[%s]\t%s = %s", section, key, value);
    }
    return 1;
}

void makeIni()
{

    int key;
    char cmd[SIZE_ITEM_S];
    char tmp_ini[SIZE_ITEM_S];
    char tmp_itm[SIZE_ITEM_S];

    sprintf(tmp_ini, "./%s_XXXXXX", _APP_NAME_);
    mkstemp(tmp_ini);
    strcpy(tmp_itm, "<place_holder>");

    // Write config of INPUT Section
    for ( key = 0; key < E_NOF_PAR_INPUT; key++ ) {
        ini_puts(gszIniStrSection[E_INPUT], gszIniStrOutput[key], tmp_itm, tmp_ini);
    }

    // Write config of OUTPUT Section
    for ( key = 0; key < E_NOF_PAR_OUTPUT; key++ ) {
        ini_puts(gszIniStrSection[E_OUTPUT], gszIniStrOutput[key], tmp_itm, tmp_ini);
    }

    // Write config of COMMON Section
    for ( key = 0; key < E_NOF_PAR_COMMON; key++ ) {
        ini_puts(gszIniStrSection[E_COMMON], gszIniStrCommon[key], tmp_itm, tmp_ini);
    }

    // Write config of DB Section
    for ( key = 0; key < E_NOF_PAR_DBCONN; key++ ) {
        ini_puts(gszIniStrSection[E_DBCONN_ERM], gszIniStrDbConnErm[key], tmp_itm, tmp_ini);
    }

    sprintf(cmd, "mv %s %s.ini", tmp_ini, tmp_ini);
    system(cmd);
    fprintf(stderr, "ini template file is %s.ini\n", tmp_ini);

}

void clearCksum(char *file)
{
    char cksum_prev[SIZE_ITEM_M], cksum_curr[SIZE_ITEM_M], cmd[SIZE_ITEM_L];
    char tmp_file[SIZE_ITEM_L], f_dir[SIZE_ITEM_M], f_name[SIZE_ITEM_M];

    strcpy(tmp_file, file);
    strcpy(f_dir, dirname(tmp_file));
    strcpy(tmp_file, file);
    strcpy(f_name, basename(tmp_file));

    sprintf(cksum_prev, "%s/.%s.cks_prev", f_dir, f_name);
    sprintf(cksum_curr, "%s/.%s.cks_curr", f_dir, f_name);

    sprintf(cmd, "rm -f %s", cksum_prev);
    system(cmd);
    sprintf(cmd, "rm -f %s", cksum_curr);
    system(cmd);
}