#!/usr/bin/ksh
INCLUDES="-I. -I/usr/include -I../../include"
CC=gcc
PROC=proc
UNAME=`uname`
if [ ${UNAME} = "HP-UX" ]; then
    CFLAGS="-g -Wall -DPORTABLE_STRNICMP"
else
    CFLAGS="-g -Wall -m64"
fi

#LINK_LIB=/usr/lib/hpux64/libm.a -L$(ORACLE_HOME)/lib -mt -lpthread -lclntsh
PROCINCLUDE="include=../../include"
#PROCFLAGS="lines=yes sqlcheck=syntax mode=oracle maxopencursors=200 dbms=v8 DEFINE=__HPUX_ ${PROCINCLUDE}"
PROCFLAGS="lines=yes sqlcheck=syntax mode=oracle maxopencursors=200 dbms=v8 ${PROCINCLUDE}"
ORACLE_INCLUDES="-I${ORACLE_HOME}/precomp/public -I${ORACLE_HOME}/rdbms/public"

BIN_DIR=./bin
OBJ_DIR=./obj
LIB_DIR=../../libs/c

echo "${CC} ${CFLAGS} -o ${OBJ_DIR}/procsig.o   -c ${LIB_DIR}/procsig.c   ${INCLUDES}"
      ${CC} ${CFLAGS} -o ${OBJ_DIR}/procsig.o   -c ${LIB_DIR}/procsig.c   ${INCLUDES}
echo "${CC} ${CFLAGS} -o ${OBJ_DIR}/strlogutl.o -c ${LIB_DIR}/strlogutl.c ${INCLUDES}"
      ${CC} ${CFLAGS} -o ${OBJ_DIR}/strlogutl.o -c ${LIB_DIR}/strlogutl.c ${INCLUDES}
echo "${CC} ${CFLAGS} -o ${OBJ_DIR}/minIni.o    -c ${LIB_DIR}/minIni.c    ${INCLUDES}"
      ${CC} ${CFLAGS} -o ${OBJ_DIR}/minIni.o    -c ${LIB_DIR}/minIni.c    ${INCLUDES}
echo "${CC} ${CFLAGS} -o ${OBJ_DIR}/snapfile.o  -c ${LIB_DIR}/snapfile.c  ${INCLUDES}"
      ${CC} ${CFLAGS} -o ${OBJ_DIR}/snapfile.o  -c ${LIB_DIR}/snapfile.c  ${INCLUDES}
echo "${CC} ${CFLAGS} -o ${OBJ_DIR}/frm_geo_feed.o  -c ./frm_geo_feed.c           ${INCLUDES}"
      ${CC} ${CFLAGS} -o ${OBJ_DIR}/frm_geo_feed.o  -c ./frm_geo_feed.c           ${INCLUDES}
echo "${PROC} ${PROCFLAGS} frm_geo_feed_dbu.pc"
      ${PROC} ${PROCFLAGS} frm_geo_feed_dbu.pc
echo "${CC} -o ${OBJ_DIR}/frm_geo_feed_dbu.o ${CFLAGS} ${ORACLE_INCLUDES} ${INCLUDES} -c ./frm_geo_feed_dbu.c"
      ${CC} -o ${OBJ_DIR}/frm_geo_feed_dbu.o ${CFLAGS} ${ORACLE_INCLUDES} ${INCLUDES} -c ./frm_geo_feed_dbu.c
echo "${CC} ${CFLAGS} -lm -L${ORACLE_HOME}/lib -lclntsh -o ${BIN_DIR}/frm_geo_feed.exe ${OBJ_DIR}/minIni.o ${OBJ_DIR}/strlogutl.o ${OBJ_DIR}/procsig.o ${OBJ_DIR}/snapfile.o ${OBJ_DIR}/frm_geo_feed.o ${OBJ_DIR}/frm_geo_feed_dbu.o"
      ${CC} ${CFLAGS} -lm -L${ORACLE_HOME}/lib -lclntsh -o ${BIN_DIR}/frm_geo_feed.exe ${OBJ_DIR}/minIni.o ${OBJ_DIR}/strlogutl.o ${OBJ_DIR}/procsig.o ${OBJ_DIR}/snapfile.o ${OBJ_DIR}/frm_geo_feed.o ${OBJ_DIR}/frm_geo_feed_dbu.o
echo "rm -f ${OBJ_DIR}/strlogutl.o ${OBJ_DIR}/minIni.o ${OBJ_DIR}/frm_geo_feed.o ${OBJ_DIR}/procsig.o ${OBJ_DIR}/snapfile.o ${OBJ_DIR}/frm_geo_feed_dbu.o ./frm_geo_feed_dbu.c ./frm_geo_feed_dbu.lis"
      rm -f ${OBJ_DIR}/strlogutl.o ${OBJ_DIR}/minIni.o ${OBJ_DIR}/frm_geo_feed.o ${OBJ_DIR}/procsig.o ${OBJ_DIR}/snapfile.o ${OBJ_DIR}/frm_geo_feed_dbu.o ./frm_geo_feed_dbu.c ./frm_geo_feed_dbu.lis
