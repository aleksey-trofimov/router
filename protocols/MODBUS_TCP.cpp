#include "MODBUS_TCP.h"
#include "../ServerSocket.h"
#include "../Message.h"

CMODBUS_TCP::CMODBUS_TCP(	CMessageQueue* pmqRouter_queue,
				CMessageQueue* pmqNode_queue,
				CDriver* pdDriver,
				CAddress aAddress)
:CProtocol(pmqRouter_queue, pmqNode_queue, pdDriver, aAddress)
{
    _iProtocol = PROTOCOL_MODBUS_TCP;
}

CMODBUS_TCP::~CMODBUS_TCP()
{
}

void CMODBUS_TCP::start_protocol()
{
    pthread_t thread_ethernet;
    pthread_create(&thread_ethernet, 0, run_thread, (void*)this);
    pthread_detach(thread_ethernet);
}

void* CMODBUS_TCP::run_thread(void* param)
{
    INFO("CMODBUS_TCP run_thread thread started!");

    CMODBUS_TCP* pProtocol = (CMODBUS_TCP*)param;

    
//Init section    
    int			report = 0;
    BYTE		local_address = 0;
    BYTE		priority = 0;
    CDataPacket*	data_packet;
    CMessage*		message;
    CDataMessage*	data_message;
    CCmdMessage*	cmd_message;
    vector<BYTE>	data;
    
//Trying to receive client message
    while(1)
    {
	data.clear();
	
	report = pProtocol->ReceiveData(data, MAX_MODBUS_TCP_PDU);

	//Message processing/////////////////////////////////

	if(report > 0) //Some data was received
	{
	    TEXTOUT("MODBUS_TCP: Received from TM:",data);
	    cout<<"MODBUS_TCP: Received by local node: "<<pProtocol->getAddress().GetLAddress()<<"\n";
	    //Extract local address from data
	    local_address = data[6];
	    //Message is addressed to local_address
	    //Assuming that we have only one controller and message is addressed
	    //to local_address this controller.
	    CAddress destination(pProtocol->_aAddress.GetCID(), local_address);
	    
//Assuming that all messages are DataMessages
	    if(local_address < MAX_LOCAL_ADDRESS)
	    {
		cout<<"Message for local address: "<<(int)local_address<<"\n";
		data_packet = new CDataPacket (data);
		data_message = new CDataMessage (data_packet, pProtocol->_aAddress, destination, pProtocol->GetProtocolCode(), priority);
		//Put message into router queue;
		
		pProtocol->GetRouterQueue()->put(data_message);
		pProtocol->GetRouterQueue()->Unlock();				
		INFO("MODBUS_TCP: message was sent to router");
	    }
	    else
	    {
		INFO("MODBUS_TCP: Too large local address received. Check config.");
	    }
	}
	else
	{
	    INFO("MODBUS_TCP: Socket error. Exiting from thread...");
	    delete pProtocol;
	    return 0;
	}	    
//Trying to get answer and transmit it to SCADA
	//Wait for answer in node queue
	pProtocol->GetNodeQueue()->Lock();
	do
	{	    
	    //Get message from node queue
	    message = pProtocol->GetNodeQueue()->get();
	    //Check message type
	    
	    if(dynamic_cast<CDataMessage*>(message))
	    {
		INFO("MODBUS_TCP: Possibly MPSU answer received by MODBUS driver");
		data_message = dynamic_cast<CDataMessage*>(message);
		//Data message processing
		//Convert protocols if required		
		pProtocol->Convert(data_message, PROTOCOL_MODBUS_TCP);
		
// Will I need this section in future?
		vector<BYTE> test;
		test = data_message->getPacket()->getData();
		if( ((test[0] == 0) && (test.size() == 1)) )
		{
		    vector<BYTE> initial;
		    initial = data_message->getInitialPacket()->getData();
		    int function_code = (int)initial[7];
		    if(function_code == 3)
		    {
		    	initial.erase(initial.begin()+7, initial.end());
    			initial[5] = 3;
    			initial.push_back(ERROR_CODE_3);
			initial.push_back(1);
		    }
		    if(function_code == 6)
		    {
		    	initial.erase(initial.begin()+7, initial.end());
    			initial[5] = 3;
    			initial.push_back(ERROR_CODE_6);
			initial.push_back(1);
		    }
		    data_message->getPacket()->setData(initial);
		}
		//Send message by interface
		if(pProtocol->SendData(data_message->getPacket()->getData(),1))
		{
		    TEXTOUT("MODBUS_TCP: Data was sent to SCADA: ", data_message->getPacket()->getData());
		    //Delete used data_message
		    delete data_message;
		}
		else
		{
		    INFO("MODBUS_TCP: Unable to send data to socket. Clothing thread...");
		    delete pProtocol;
		    //Delete used data_message
		    delete data_message;
		    return 0;
		}		
	    }
	    else if(dynamic_cast<CCmdMessage*>(message))
	    {
		//Command message processing
		INFO("MODBUS_TCP: CmdMessage message type received");
		cmd_message = dynamic_cast<CCmdMessage*>(message);
		pProtocol->GetDriver()->ProcessCmd(cmd_message);
		delete cmd_message;
	    }	    
	    else
	    {
		INFO("MODBUS_TCP: ERROR! Unknown message type received");
	    }
	
	}while(!(pProtocol->GetNodeQueue()->Empty()));	
    }	
}

int CMODBUS_TCP::SendData(vector<BYTE> data, int timeout)
{
    return GetDriver()->WriteData(data, timeout);
}

int CMODBUS_TCP::ReceiveData(vector<BYTE>& data, int size, int timeout)
{
    return GetDriver()->ReadData(data, size, timeout);
}
