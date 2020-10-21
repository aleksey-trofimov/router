#if !defined ProtocolCONFIG
#define ProtocolCONFIG

#include "Protocol.h"
#include <algorithm>

class CProtocolCONFIG : public CProtocol
{
public:
        CProtocolCONFIG(CMessageQueue* pmqRouter_queue,
			CMessageQueue* pmqNode_queue,
			CDriver* pdDriver,
			CAddress aAddress);
        ~CProtocolCONFIG();
	
	virtual int SendData(vector<BYTE> data, int timeout = 0);
	virtual int ReceiveData(vector<BYTE>& data, int size, int timeout = 0);
	
//	virtual int GetAddress(vector<BYTE> data, int timeout);
	
/////THREAD FUNCTIONS///////
	void start_protocol();
	static void* run_thread(void*);
///////////////////////////
		
};

#endif
