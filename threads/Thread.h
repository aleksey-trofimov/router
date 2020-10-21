#if !defined Thread_h
#define Thread_h

#include "../ServerSocket.h"
#include "../protocols/Protocol.h"
#include "../protocols/ProtocolMPSU.h"
#include "../protocols/ProtocolMOUSE.h"
#include "../protocols/ProtocolCONFIG.h"
#include "../protocols/MODBUS_TCP.h"
#include "../drivers/Driver.h"
#include "../drivers/MPSUSerialDriver.h"
#include "../drivers/EthernetTCPClient.h"
#include "../system_data.h"

class CThread
{
public:
        CThread();
        virtual ~CThread() = 0;
	
	CProtocol* CreateProtocol(CDriver* driver);

    struct TRANSFER
    {
	SYS_DATA*		_psSysData;
	int			_iProtocolCode;
	int			_iPort;
    }_data;
};
#endif
