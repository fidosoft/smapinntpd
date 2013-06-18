#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#ifdef TLSENABLED
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#endif

#ifndef PLATFORM_WIN32
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
typedef int SOCKET;
#endif

#ifndef PLATFORM_LINUX
typedef unsigned long ulong;
typedef unsigned short ushort;
#endif

typedef int bool;
typedef unsigned char uchar;

#include <smapi/msgapi.h>
//#include <smapi/progprot.h>

struct var
{
   struct sockio *sio;
   bool disconnect;

   struct group *currentgroup;
   ulong currentarticle;

   struct group *opengroup;
   HAREA openarea;

   ulong inputpos;
   uchar *input;

   uchar clientid[100];

   struct group *firstgroup;

   struct xlat *firstreadxlat;
   struct xlat *firstpostxlat;
   struct xlatalias *firstreadalias;
   struct xlatalias *firstpostalias;
   struct xlattab *firstxlattab;

   uchar defaultreadchrs[20];
   uchar defaultpostchrs[20];
   
   uchar readgroups[50];
   uchar postgroups[50];

   uchar loginname[100];
   uchar password[100];
   uchar realnames[100];
   
   bool opt_flowed;
   bool opt_showto;

   bool login;
};

#include "os.h"
#include "groups.h"
#include "misc.h"
#include "xlat.h"
#include "allow.h"
#include "sockio.h"
#include "login.h"
#include "mime.h"

#define CR "\x0d"
#define LF "\x0a"

#define CRLF CR LF

#define SERVER_NAME       "JamNNTPd-SMAPI/" PLATFORM_NAME
#define SERVER_VERSION    "1.2.1"

#define SOCKIO_TIMEOUT 5*60

extern int server_openconnections;
extern int server_quit;

int get_server_openconnections(void);
int get_server_quit(void);

void server(SOCKET s);

#define CONFIGFILE         CFG_BASEPATH "jamnntpd.config"

#define CFG_PORT           5000
#define CFG_MAXCONN        5

#define CFG_ALLOWFILE      CFG_BASEPATH "jamnntpd.allow"
#define CFG_GROUPSFILE     CFG_BASEPATH "jamnntpd.groups"
#define CFG_USERSFILE      CFG_BASEPATH "jamnntpd.users"
#define CFG_XLATFILE       CFG_BASEPATH "jamnntpd.xlat"

#define CFG_LOGFILE        LOG_BASEPATH "jamnntpd.log"

#define CFG_DEF_FLOWED     TRUE
#define CFG_DEF_SHOWTO     TRUE

extern ulong cfg_port;
extern ulong cfg_maxconn;
extern uchar *cfg_allowfile;
extern uchar *cfg_groupsfile;
extern uchar *cfg_logfile;
#ifdef TLSENABLED
extern uchar *cfg_certfile;
#endif
extern uchar *cfg_usersfile;
extern uchar *cfg_xlatfile;

extern uchar *cfg_origin;
extern uchar *cfg_guestsuffix;
extern uchar *cfg_echomailjam;

extern bool cfg_debug;
extern bool cfg_noxlat;
extern bool cfg_noecholog;
extern bool cfg_nostripre;
extern bool cfg_notearline;
extern bool cfg_noreplyaddr;
extern bool cfg_smartquote;
extern bool cfg_noencode;
extern bool cfg_notzutc;
extern bool cfg_nocancel;
extern bool cfg_strictnetmail;
extern bool cfg_readorigin;

extern bool cfg_def_flowed;
extern bool cfg_def_showto;
