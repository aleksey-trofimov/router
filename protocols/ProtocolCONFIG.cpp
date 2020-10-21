#include "ProtocolCONFIG.h"

CProtocolCONFIG::CProtocolCONFIG(CMessageQueue* pmqRouter_queue,
				CMessageQueue* pmqNode_queue,
				CDriver* pdDriver,
				CAddress aAddress)
:CProtocol(pmqRouter_queue, pmqNode_queue, pdDriver, aAddress)
{
    _iProtocol = PROTOCOL_CONFIG;
}

CProtocolCONFIG::~CProtocolCONFIG()
{
}

int CProtocolCONFIG::SendData(vector<BYTE> data, int timeout)
{
    
    INFO("CProtocolCONFIG::SendData executed. Not supported.");
    return 0;
}

int CProtocolCONFIG::ReceiveData(vector<BYTE>& data, int size, int timeout)
{
    const int CONFIG_TIMEOUT = 1;
//    INFO("CProtocolCONFIG::ReceiveData started...");
    vector<BYTE> data_to_receive(size);
    data.clear();
    
    //Try to receive 10 bytes from port
    if((_pdDriver->ReadData(data_to_receive, size, CONFIG_TIMEOUT)) != -1)
    {
	data =  data_to_receive;
//	TEXTOUT("Data received from CONFIG: ", data);
	return 1;
    }   
    else
	return 0;
}

/*int CProtocolCONFIG::GetAddress(vector<BYTE> data, int timeout)
{
// Function is not used in MPSU protocol because MPSU can not initiate
// data exchange
    return 0;
}*/

void CProtocolCONFIG::start_protocol()
{
    pthread_t thread_serial;
    pthread_create(&thread_serial, 0, run_thread, (void*)this);
    pthread_detach(thread_serial);    
}

void* CProtocolCONFIG::run_thread(void* param)
{
    const int MAXIMUM_SIZE = 2;
    BYTE iDestinationAddress = 0;
    CProtocolCONFIG* protocol;
    protocol = reinterpret_cast<CProtocolCONFIG*>(param);
    CMessage* message;
    CDataPacket* data_packet;
    CCmdMessage* cmd_message;
    CDataMessage* data_message;
    int priority = 0;
    BYTE local_address = 0;
    int error = 0;
    int size = 0;
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
		INFO("CONFIG: DataMessage message type received. Sending message back.");
		data_message = dynamic_cast<CDataMessage*>(message);
		//Send back blank message		
    		vector<BYTE> answer;
		answer.push_back(0);
		data_message->getPacket()->setData(answer);
		//Set address fields
		data_message->setTo( data_message->getFrom() );
		data_message->setFrom( protocol->_aAddress );
		//Send message to router
		protocol->GetRouterQueue()->put(data_message);
		protocol->GetRouterQueue()->Unlock();				
		data_message = 0;
		INFO("CONFIG: data_message was sent back to router");
	    }
    	    else if(dynamic_cast<CCmdMessage*>(message))
    	    {
		//Command message processing
		INFO("CONFIG: CmdMessage message type received");
		protocol->GetDriver()->ProcessCmd(message);
	    }	    
	    else
	    {
		INFO("CONFIG: Unknown message type received");
	    }
	}
	//Message processing complete. Now we can receive data from port
	vector<BYTE> rdata;
	size = 0;
////////////////////////////////////////////////////////////////////////////////
//Command message format:
//1 byte: size of the rest of message(for now, always=2)
//2 byte: destination node
//3 byte: protocol to use in node

	//Receive first byte(size) of command message
//	INFO("CONFIG: Attempting to receive first byte of command message");
	if(protocol->ReceiveData(rdata,1))
	{
	    size = (int)rdata[0];
	    
	    if(protocol->ReceiveData(rdata,size))
	    {
		TEXTOUT("Received in CONFIG: ",rdata);
		cout<<"CONFIG: Received by local node: "<<protocol->getAddress().GetLAddress()<<"\n";
		//Extract destination node address from received data
		iDestinationAddress = rdata[0];
		//Create command string by deleting first byte from rdata
		rdata.erase( rdata.begin() );		
		TEXTOUT("CONFIG: After extraction: ",rdata);
		//Set destination address
		CAddress destination(protocol->_aAddress.GetCID(), iDestinationAddress);
		//Create data packet and Command message	    	    
		data_packet = new CDataPacket (rdata);
		cmd_message = new CCmdMessage (data_packet, protocol->_aAddress, destination, priority);
		//Put message into router queue
		protocol->GetRouterQueue()->put(cmd_message);
		protocol->GetRouterQueue()->Unlock();				
		INFO("CONFIG: message was sent to router");
	    }	
	    else
	    {
		INFO("CONFIG: Unable to receive the rest of command message");
	    }
	}
	else
	{
	    //INFO("CONFIG: Unable to receive first byte of command message");
	}
    } while(1);    
}
