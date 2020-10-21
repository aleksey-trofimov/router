#include "ProtocolMOUSE.h"
#include "ProtocolMPSU.h"
#include "../Socket.h"


CProtocolMOUSE::CProtocolMOUSE(	CMessageQueue* pmqRouter_queue,
				CMessageQueue* pmqNode_queue,
				CDriver* pdDriver,
				CAddress aAddress)
:CProtocol(pmqRouter_queue, pmqNode_queue, pdDriver, aAddress)
{
    _iProtocol = PROTOCOL_MOUSE;
}

CProtocolMOUSE::~CProtocolMOUSE()
{
}

int CProtocolMOUSE::SendData(vector<BYTE> data, int timeout)
{
    
    INFO("MOUSE::SendData executed. Must be error.");
    return 0;
}

int CProtocolMOUSE::ReceiveData(vector<BYTE>& data, int size, int timeout)
{
    const int MOUSE_TIMEOUT = 1;
    const int MOUSE_BUFFER = 100;
//    INFO("CProtocolMOUSE::ReceiveData started...");
    vector<BYTE> data_to_receive(MOUSE_BUFFER);
    data.clear();
    
    //Try to receive 10 bytes from port
    if((_pdDriver->ReadData(data_to_receive, MOUSE_BUFFER, MOUSE_TIMEOUT)) != -1)
    {
	data =  data_to_receive;
//	TEXTOUT("MOUSE: Data received from MOUSE: ", data);
	return 1;
    }   
    else
	return 0;
}

/*int CProtocolMOUSE::GetAddress(vector<BYTE> data, int timeout)
{
// Function is not used in MPSU protocol because MPSU can not initiate
// data exchange
    return 0;
}*/

void CProtocolMOUSE::start_protocol()
{
    pthread_t thread_serial;
    pthread_create(&thread_serial, 0, run_thread, (void*)this);
    pthread_detach(thread_serial);    
}

void* CProtocolMOUSE::run_thread(void* param)
{
    const int MOUSE_CLIENT_NODE_ADDRESS = 100;
    CProtocolMOUSE* protocol;
    protocol = reinterpret_cast<CProtocolMOUSE*>(param);
    CMessage* message;
    CDataMessage* data_message;
    CDataPacket* data_packet;
    CCmdMessage* cmd_message;
    int priority = 0;
    BYTE local_address = 0;
    int error = 0;
    vector<BYTE> initial_data;    
    
    do
    {
    	//Try to receive command message from node queue
	message = 0;
	message = protocol->GetNodeQueue()->get();
	//Check message type	    
	if(message != 0)
	{
	    if(dynamic_cast<CDataMessage*>(message))
	    {
		INFO("MOUSE: DataMessage message type received. Sending message back.");
		data_message = dynamic_cast<CDataMessage*>(message);
		//Send back blank message
		//Create error message
    		vector<BYTE> answer;
		answer.push_back(0);
		data_message->getPacket()->setData(answer);
		//Send back this message.
		data_message->setTo( data_message->getFrom() );
		data_message->setFrom( protocol->_aAddress );
		protocol->GetRouterQueue()->put(data_message);
		protocol->GetRouterQueue()->Unlock();				
		INFO("MOUSE: data_message was sent back to router");
	    }
    	    else if(dynamic_cast<CCmdMessage*>(message))
    	    {
		//Command message processing
		INFO("MOUSE: CmdMessage message type received");
		cmd_message = dynamic_cast<CCmdMessage*>(message);
		data_packet = cmd_message->getCommand();
		TEXTOUT("MOUSE: Received command message:", data_packet->getData());
		//Create data for new protocol
		int device = protocol->GetDriver()->GetDevice();
		CMessageQueue* r_queue = protocol->GetRouterQueue();
		CMessageQueue* n_queue = protocol->GetNodeQueue();
		CAddress a_address = protocol->getAddress();
		//Delete old protocol
		protocol->GetNodeQueue()->Free();
		delete protocol;
		//Create new protocol
		CMPSUSerialDriver* new_driver = new CMPSUSerialDriver(device);
		
		CProtocolMPSU* new_protocol = new CProtocolMPSU(
					    r_queue,
					    n_queue,
					    new_driver,
					    a_address);
		new_protocol->GetNodeQueue()->Use();
		new_protocol->start_protocol();
		delete message;
		return 0;
	    }	    
	    else
	    {
		INFO("MOUSE: Unknown message type received");
	    }
	}
	//Message processing complete. Now we can receive data from port
	vector<BYTE> rdata;
	if(protocol->ReceiveData(rdata,10))
	{
	    cout<<"MOUSE: Received by local node: "<<protocol->getAddress().GetLAddress()<<"\n";
	    //Make Ethernet connection and send received data
	    {
		Socket socket;
		socket.create();
		if( socket.connect("192.168.1.109",500) )
		    socket.send(rdata);
	    }
	    
        }	
	
    } while(1);    
}
