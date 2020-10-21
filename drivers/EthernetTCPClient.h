#if !defined EthernetTCPClient
#define EthernetTCPClient

#include "../ServerSocket.h"
#include "Driver.h"


class CEthernetTCPClient : public CDriver
{
public:
        CEthernetTCPClient(ServerSocket* socket);
        ~CEthernetTCPClient();

	int ReadData (vector<BYTE>& data, int size, int timeout = 0);
        int WriteData(const vector<BYTE>& data, int timeout = 0);

	int ProcessCmd(CMessage*){}
	
private:
    ServerSocket* _pssSocket;
};
#endif
