#if !defined ProtocolMPSU
#define ProtocolMPSU

#include "Protocol.h"
#include "MPSURecord.h"
#include <algorithm>
//Protocol MPSU constants

const BYTE start_packet_character=0x55;
const BYTE finish_packet_character=0xAA;
const BYTE system_byte=0x56;

class CProtocolMPSU : public CProtocol
{
public:
        CProtocolMPSU(	CMessageQueue* pmqRouter_queue,
			CMessageQueue* pmqNode_queue,
			CDriver* pdDriver,
			CAddress aAddress);
        ~CProtocolMPSU();
	
	virtual int SendData(vector<BYTE> data, int timeout = 0);
	virtual int ReceiveData(vector<BYTE>& data, int size, int timeout = 0);
	
//	virtual int GetAddress(vector<BYTE> data, int timeout);

	void ClearPort();
	int isCacheable(vector<BYTE> data);
	
	void String2Protocol(vector<BYTE>& data);
	void Protocol2String(vector<BYTE>& data);
	
	void AddSystemBytes(CDataMessage* data_message);
	void RemoveSystemBytes(CDataMessage* data_message);
	
	struct CACHE_STARTUP
	{
	    map< vector<BYTE>, CACHE_REQUEST >*	_pmCache;
	    void*				_pProtocol;
	};
	static void update_record(pair<const vector<BYTE>, CACHE_REQUEST >);
	
/////THREAD FUNCTIONS///////
	void start_protocol();
	static void* run_thread(void*);
	static void* run_cache(void*);
///////////////////////////
		
};

#endif
