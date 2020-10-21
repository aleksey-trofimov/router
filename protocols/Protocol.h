#if !defined Protocol
#define Protocol

#include "../global.h"
#include <vector>
#include "../Address.h"
#include "../MessageQueue.h"
#include "../Message.h"
#include "../Router.h"
#include "../drivers/Driver.h"
#include "../drivers/MPSUSerialDriver.h"
#include "MPSURecord.h"
#include <pthread.h>

//MODBUS TCP function codes
const BYTE READ_COILS				= 0x01;
const BYTE READ_INPUT_DISCRETES			= 0x02;
const BYTE READ_MULTIPLE_REGISTERS		= 0x03;
const BYTE READ_INPUT_REGISTERS			= 0x04;
const BYTE WRITE_COIL				= 0x05;
const BYTE WRITE_SINGLE_REGISTER		= 0x06;
const BYTE WRITE_MULTIPLE_REGISTERS		= 0x10;
const BYTE FORCE_MULTIPLE_COILS			= 0x0F;

//MODBUS TCP exception codes
const BYTE ILLEGAL_FUNCTION			= 0x01;
const BYTE ILLEGAL_DATA_ADDRESS			= 0x02;
const BYTE ILLEGAL_DATA_VALUE			= 0x03;
const BYTE ILLEGAL_RESPONSE_LENGTH		= 0x04;
const BYTE ACKNOWLEDGE				= 0x05;
const BYTE SLAVE_DEVICE_BUSY			= 0x06;
const BYTE NEGATIVE_ACKNOWLEDGE			= 0x07;
const BYTE MEMORY_PARITY_ERROR			= 0x08;
const BYTE GATEWAY_PATH_UNAVAILABLE		= 0x0A;
const BYTE GATEWAY_TARGET_FAILED_TO_RESPOND	= 0x0B;

//MODBUS TCP error codes for specific functions
const BYTE ERROR_CODE_3				= 0x83;
const BYTE ERROR_CODE_6				= 0x86;

//Zero point for analog-digital conversion in MPSU-MODBUS_TCP
const int ANALOG_ZERO = 2048;

class CProtocol
{
public:
CProtocol(CMessageQueue*, CMessageQueue*, CDriver*, CAddress);
//        Router queue    Node queue      Driver    Node
        ~CProtocol();
	
	CDriver* GetDriver(){return _pdDriver;}
	CAddress getAddress(){return _aAddress;}
	
	virtual int SendData(vector<BYTE> data, int timeout=0) = 0;
	virtual int ReceiveData(vector<BYTE>& data, int size, int timeout=0) = 0;
	
	CMessage* OutNodeQueue();
	void InRouterQueue(CMessage* msg);//Possibly not working. Why?
	
	CMessageQueue* GetNodeQueue();
	CMessageQueue* GetRouterQueue();
	
	int GetProtocolCode(){return _iProtocol;}
	
	int Convert(CDataMessage*, int);
	
	int MODBUS_TCP2MPSU(vector<BYTE>& modbus_data);
	short Analog2Digit(short analog);
	short Digit2Analog(short digit);

	int MPSU2MODBUS_TCP(vector<BYTE>& MPSU_answer, vector<BYTE> modbus_request);
		
/////THREAD FUNCTION///////
	virtual void start_protocol() = 0;
///////////////////////////
	struct CACHE_REQUEST
	{
	    CACHE_REQUEST(){_iCounter = 0;}
	    	    
	    vector<BYTE> _vbAnswer;
	    vector<BYTE> _vbRequest;
	    int		 _iCounter;
	};
		
protected:
    CMessageQueue*	_pmqRouter_queue;
    CMessageQueue*	_pmqNode_queue;
    CDriver*		_pdDriver;    
    CAddress		_aAddress;
    int			_iProtocol;
};

#endif
