#include "EthernetTCPServer.h"

CEthernetTCPServer::CEthernetTCPServer(SYS_DATA* psSysData, int iProtocolCode, int port)
{
    _data._psSysData		= psSysData;
    _data._iProtocolCode	= iProtocolCode;
    _data._iPort 		= port;

    //Start server thread
    pthread_t server_thread;
    pthread_create( &server_thread, 0, run_server, (void*)this);
    //&_data);
    pthread_detach(server_thread);    
}

CEthernetTCPServer::~CEthernetTCPServer()
{
}

void* CEthernetTCPServer::run_server(void* t_data)
{
    INFO("EthernetTCPServer thread started!");
    CEthernetTCPServer* server = (CEthernetTCPServer*)t_data;
    ServerSocket server_socket(server->_data._iPort);
    while ( true )
    {    
	//Accept incoming connections
	ServerSocket* new_sock = new ServerSocket;	
	server_socket.accept ( *new_sock );
	//Create client driver object
	CEthernetTCPClient* client_driver = new CEthernetTCPClient(new_sock);
	//Create protocol object using factory
	CProtocol* protocol = server->CreateProtocol(client_driver);
	//Start client thread	
	protocol->start_protocol();

    }
}
