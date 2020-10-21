#ifndef _definitions_include_file_
#define _definitions_include_file_
//Common includes
#include <vector>
#include <map>
#include <string>
#include <queue>
#include <iostream>

#include "sys/types.h"
#include "sys/stat.h"
#include "sys/time.h"
#include <sys/param.h>
#include <syslog.h>
#include "sys/select.h"
#include <errno.h>
#include <fcntl.h>
#include "unistd.h"
#include <stdio.h>

#define DEBUG_MODE
//#define CACHE_ENABLED

using namespace std;

typedef unsigned char BYTE;
typedef char WORD;
typedef unsigned long  DWORD;

typedef int SOCKETNUM;
typedef unsigned char PRIORITY;
typedef unsigned char GENTYPE;

const int MAX_CLIENTS=8;

//Timeouts//////////////////////////////////
const int MPSU_TIMEOUT=1;//1sec
const int MPSU_CHAR_TIMEOUT=1;//1sec
const int CACHE_DELAY=1000;//0.001sec
////////////////////////////////////////////
//Number of used ports. Do you need this?
//const int LEGAL_PORTS_NUMBER=2;
//Number of times that cache will query device
const int CACHE_COUNTS=5;

//Protocols:
const int PROTOCOL_CONFIG	= 0;
const int PROTOCOL_MPSU		= 1;
const int PROTOCOL_MODBUS_TCP	= 2;
const int PROTOCOL_MOUSE	= 3;

//Interface types
const int SERIAL_PORT	= 1;
const int PARALLEL_PORT = 2;
const int ETHERNET	= 3;

//Device numbers
const int MAX_SERIAL_PORT	= 32;
const int MAX_PARALLEL_PORT	= 32;
const int MAX_ETHERNET		= 100;

const int MAX_LOCAL_ADDRESS	= 164;
const int MAX_MODBUS_TCP_PDU	= 256;

#ifdef DEBUG_MODE
#define INFO(text) cout<<#text<<"\n";
#endif

#ifndef DEBUG_MODE
#define INFO(text)
#endif

#endif
