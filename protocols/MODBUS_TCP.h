#if !defined MODBUS_TCP
#define MODBUS_TCP

#include "Protocol.h"
#include "../drivers/EthernetTCPClient.h"

//Maximum message size for MODBUS_TCP protocol
const int MODBUS_TCP_MAX_MESSAGE = 1000;

class CMODBUS_TCP : public CProtocol
{
public:
        CMODBUS_TCP(	CMessageQueue* pmqRouter_queue,
			CMessageQueue* pmqNode_queue,
			CDriver* pdDriver,
			CAddress aAddress);
        ~CMODBUS_TCP();
	
	virtual int SendData(vector<BYTE> data, int timeout=0);
	virtual int ReceiveData(vector<BYTE>& data, int size, int timeout=0);
	
//	int Convert(CDataMessage*, int);
	
/////THREAD FUNCTION///////
	static void* run_thread(void* param);

	void start_protocol();
///////////////////////////

};

#endif
