#if !defined EthernetTCPServer_h
#define EthernetTCPServer_h

#include "../ServerSocket.h"
#include "../protocols/Protocol.h"
#include "../drivers/Driver.h"
#include "../system_data.h"
#include "Thread.h"

class CEthernetTCPServer : public CThread
{
public:
        CEthernetTCPServer(SYS_DATA* psSysData, int iProtocolCode, int port);
        ~CEthernetTCPServer();
	
	static void* run_server(void*);
};
#endif
