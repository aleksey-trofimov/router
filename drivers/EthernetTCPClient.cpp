#include "EthernetTCPClient.h"
#include "../SocketException.h"

CEthernetTCPClient::CEthernetTCPClient(ServerSocket* socket)
:_pssSocket(socket)
{
    SetRange(ETHERNET);
}

CEthernetTCPClient::~CEthernetTCPClient()
{
    if(_pssSocket != 0)
	delete _pssSocket;
}

int CEthernetTCPClient::ReadData(vector<BYTE>& data, int size, int timeout)
{   
    int report;
    
    try
    {

	report = _pssSocket->recv( data );

	//Return values:	-1 No message arrived
	//			0  Error 
	//			>0 Success
    }
    catch(SocketException& re)
    {
	INFO("Exception was caught! Closing socket. Must also exit from thread.");

//	sockets[place] = 0;??? Clear address of this socket from router
	delete _pssSocket;
	_pssSocket = 0;
	return(0);
    }
    
    if(report == 0)
    {
	INFO("recv() report == 0! Logout forced!");
		
//	sockets[place] = 0;??? Clear address of this socket from router
	delete _pssSocket;
	_pssSocket = 0;
	return(0);	    
    }
    return report;
}

int CEthernetTCPClient::WriteData(const vector<BYTE>& data, int timeout)
{
    try
    {
	_pssSocket->send( data );
	INFO("Data was sent to socket");
    }
    catch(SocketException& re)
    {
	INFO("Exception was caught! Closing socket. Must also exit from thread.");

//	sockets[place] = 0;??? Clear address of this socket from router
	delete _pssSocket;
	_pssSocket = 0;
	return(0);
    }    
    return 1;
}
