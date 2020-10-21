#include "SerialThread.h"

CSerialThread::~CSerialThread()
{
}

CSerialThread::CSerialThread(SYS_DATA* psSysData, int iProtocolCode, int port)
{
    //Create Protocol object and run protocol thread
    _data._psSysData		= psSysData;
    _data._iProtocolCode	= iProtocolCode;
    _data._iPort 		= port;
    
    //Create client driver object
    if(_data._iProtocolCode == PROTOCOL_MPSU)
    {
	CMPSUSerialDriver* client_driver = new CMPSUSerialDriver(_data._iPort);
	//Create protocol object using factory
	CProtocol* protocol = CreateProtocol(client_driver);
	protocol->start_protocol();
    }
    
    if(_data._iProtocolCode == PROTOCOL_MOUSE)
    {
	CMOUSESerialDriver* client_driver = new CMOUSESerialDriver(_data._iPort);
	//Create protocol object using factory
	CProtocol* protocol = CreateProtocol(client_driver);
	protocol->start_protocol();
    }    
    
    if(_data._iProtocolCode == PROTOCOL_CONFIG)
    {
	CCONFIGSerialDriver* client_driver = new CCONFIGSerialDriver(_data._iPort);
	//Create protocol object using factory
	CProtocol* protocol = CreateProtocol(client_driver);
	protocol->start_protocol();
    }
}
