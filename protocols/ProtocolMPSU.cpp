#include "ProtocolMPSU.h"
#include "ProtocolMOUSE.h"

CProtocolMPSU::CProtocolMPSU(	CMessageQueue* pmqRouter_queue,
				CMessageQueue* pmqNode_queue,
				CDriver* pdDriver,
				CAddress aAddress)
:CProtocol(pmqRouter_queue, pmqNode_queue, pdDriver, aAddress)
{
    _iProtocol = PROTOCOL_MPSU;
}

CProtocolMPSU::~CProtocolMPSU()
{
}

int CProtocolMPSU::SendData(vector<BYTE> data, int timeout)
{
    //Do not forget to get semaphore before using this function
    
    //Try to clear port
    ClearPort();
    vector<BYTE> char_to_send(1);
    int size = data.size();

    INFO("CProtocolMPSU::SendData started...");
    TEXTOUT("(In SP) Data before sending to MPSU: ", data);
    
    //Sending begin.
    for( int i=0; i<size; i++)
    {
	char_to_send[0] = data[i];	
	
	//Send char into port
	if(!(_pdDriver->WriteData(char_to_send, MPSU_CHAR_TIMEOUT)))
	{
	    INFO("Unable to send char in CProtocolMPSU::SendData");
	    return 0;
	}
	
	//Receive sent char back
	if((_pdDriver->ReadData(char_to_send, 1, MPSU_CHAR_TIMEOUT)) == -1)
	{
	    INFO("Unable to receive char in CProtocolMPSU::SendData");
	    return 0;
	}
	
	//Compare chars
	if(char_to_send[0] != data[i])
	{
	    INFO("Transmitted/received chars are not equal in CProtocolMPSU::SendData");
	    return 0;
	}
    }
    
    INFO("CProtocolMPSU::SendData OK");
    return 1;    
}

int CProtocolMPSU::ReceiveData(vector<BYTE>& data, int size, int timeout)
{
    INFO("CProtocolMPSU::ReceiveData started...");
    vector<BYTE> char_to_receive(1);
    data.clear();
    
    //Receive first 4 characters with state and size	
    for(int i=0; i<4; i++)
    {
    	if((_pdDriver->ReadData(char_to_receive, 1, MPSU_CHAR_TIMEOUT)) != -1)
	{ 
	    if(_pdDriver->WriteData(char_to_receive, MPSU_CHAR_TIMEOUT))
	    {
		data.push_back(char_to_receive[0]);
	    }
	    else 
		return 0;
	}	    
	else
	    return 0;
    }
    
    //Get data tail size from first 4 bytes
    int tail_size = (int)data[2];
        
    //Receive next [tail_size] characters
    for(int i=0; i<tail_size; i++)
    {
    	if(_pdDriver->ReadData(char_to_receive, 1, MPSU_CHAR_TIMEOUT))
	{ 
	    if(_pdDriver->WriteData(char_to_receive, MPSU_CHAR_TIMEOUT))
	    {
		data.push_back(char_to_receive[0]);
	    }
	    else 
		return 0;
	}	    
	else
	    return 0;
    }
    
    //Receive last byte. This must be "finish_packet_character"
    if(_pdDriver->ReadData(char_to_receive, 1, MPSU_CHAR_TIMEOUT))
    {
	if(char_to_receive[0] == finish_packet_character)
	{ 
	    data.push_back(char_to_receive[0]);
	}
	else
	    return 0;
    }
    else
        return 0;    

    TEXTOUT("Data received from MPSU: ", data);
    return 1;
}

/*int CProtocolMPSU::GetAddress(vector<BYTE> data, int timeout)
{
// Function is not used in MPSU protocol because MPSU can not initiate
// data exchange
    return 0;
}*/

void CProtocolMPSU::start_protocol()
{
    pthread_t thread_serial;
    pthread_create(&thread_serial, 0, run_thread, (void*)this);
    pthread_detach(thread_serial);    
}

void* CProtocolMPSU::run_thread(void* param)
{
    CProtocolMPSU* protocol;
    protocol = reinterpret_cast<CProtocolMPSU*>(param);
    CMessage* message;
    CDataMessage* data_message;
    CDataPacket* data_packet;
    CCmdMessage* cmd_message;
    int error = 0;
    int unknown = 0;
    map< vector<BYTE>, CACHE_REQUEST >* cache = new map< vector<BYTE>, CACHE_REQUEST >;
    vector<BYTE> initial_data;
    
    
#ifdef CACHE_ENABLED
    //Run cache thread//////////////////////////////////////////////////
    CACHE_STARTUP* startup = new CACHE_STARTUP;
    startup->_pmCache	= cache;
    startup->_pProtocol	= (void*)protocol;
    
    pthread_t thread_cache;
    pthread_create(&thread_cache, 0, run_cache, (void*)startup);
    pthread_detach(thread_cache);    
#endif
    
    do
    {
    	//Wait for queue semaphore release. This will happen when router thread
	//will send message to this thread.
	protocol->GetNodeQueue()->Lock();
	do
	{	    
	    error = 0;
	    unknown = 0;
	    //Get message from node queue
	    message = protocol->GetNodeQueue()->get();	    
	    //Check message type	    
	    if(dynamic_cast<CDataMessage*>(message))
	    {
		INFO("DataMessage message type received by ProtocolMPSU");
		data_message = dynamic_cast<CDataMessage*>(message);
		//Data message processing
		//Convert protocols if required
		protocol->Convert(data_message, PROTOCOL_MPSU);
		protocol->AddSystemBytes(data_message);
		
#ifdef CACHE_ENABLED
		//Check if we have answer to this request in cache
		if( protocol->isCacheable(data_message->getPacket()->getData()) )
		{		
		    if( cache->operator[](data_message->getPacket()->getData())._iCounter > 0 )
		    {
			//We have answer in cache. Copy data from cache.
			data_message->getPacket()->setData(cache->operator[](data_message->getPacket()->getData())._vbAnswer);
		    }
		}
		else
#endif
		{		    
		//Save initial data
		initial_data = data_message->getPacket()->getData();		
		//Send message by interface		
		if(protocol->SendData(data_message->getPacket()->getData(),1))
		{
		    //Receive message from interface		
		    vector<BYTE> rdata;
		    if(protocol->ReceiveData(rdata,1))
		    {
			//Write data to data_message
			data_message->getPacket()->setData(rdata);
//			protocol->RemoveSystemBytes(data_message);
		    }
		    else error = 1;
		}
		else error = 1;
		
		if(error)		
		{
		    //Create error MPSU answer
		    vector<BYTE> errdata;
		    errdata.push_back(0);
		    data_message->getPacket()->setData(errdata);
		}
		
#ifdef CACHE_ENABLED
		else
		{
		    //Try to put request to cache
		    if( protocol->isCacheable( data_message->getPacket()->getData()) )
		    {
			CACHE_REQUEST cache_request;
			cache_request._vbAnswer = data_message->getPacket()->getData();
			cache_request._vbRequest = initial_data;
			cache_request._iCounter = CACHE_COUNTS;
		    }
		}
#endif
		}
		
		//Fill address fields in data_message
		data_message->setTo(data_message->getFrom());
		data_message->setFrom(protocol->getAddress());
		//Send data_message to router
		protocol->GetRouterQueue()->put(data_message);
		protocol->GetRouterQueue()->Unlock();				
	    }
	    else
	    {
		INFO("MPSU: Not DataMessage received")
		unknown += 1;
	    }
	    if(dynamic_cast<CCmdMessage*>(message))
	    {
		//Command message processing
		INFO("MPSU: CmdMessage message type received");
		cmd_message = dynamic_cast<CCmdMessage*>(message);
		data_packet = cmd_message->getCommand();
		TEXTOUT("MPSU: Received command message:", data_packet->getData());
		//Create data for new protocol
		int device = protocol->GetDriver()->GetDevice();
		CMessageQueue* r_queue = protocol->GetRouterQueue();
		CMessageQueue* n_queue = protocol->GetNodeQueue();
		CAddress a_address = protocol->getAddress();
		//Delete old protocol
		protocol->GetNodeQueue()->Free();
		delete protocol;
		//Create new protocol
		CMOUSESerialDriver* new_driver = new CMOUSESerialDriver(device);
		
		CProtocolMOUSE* new_protocol = new CProtocolMOUSE(
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
		INFO("MPSU: Not CMDMessage received")
		unknown += 1;
	    }
	    //Message processing complete.
	    if(unknown == 2)
	    {
		INFO("MPSU: Unknown message received!");
		if(message !=0 )
		    INFO("MPSU: Warning! Not zero message of unknown type received!")
		else
		    INFO("MPSU: Warning! Zero message received!")
	    }
	
	}while(!(protocol->GetNodeQueue()->Empty()));
	
    } while(1);    
}

void CProtocolMPSU::String2Protocol(vector<BYTE>& m_data)
//String stored in m_data will be converted to protocol string
{
	vector<BYTE> n_data;

        n_data.push_back(system_byte);
	n_data.push_back(start_packet_character);
	
        for(int m = 0; m<m_data.size(); m++)
        {
                if(m_data[m] == system_byte)
                {
                        n_data.push_back(system_byte);
                }

                n_data.push_back(m_data[m]);
        }

        n_data.push_back(system_byte);
        n_data.push_back(finish_packet_character);
	
	m_data = n_data;	
}

void CProtocolMPSU::Protocol2String(vector<BYTE>& m_data)
//String stored in m_data will be converted from protocol
{
	//Erase protocol header and ending bytes
	m_data.erase(m_data.end()-1);
	m_data.erase(m_data.end()-1);
	
        for(int m = 0; m<m_data.size(); m++)
        {
	    if(m_data[m] == system_byte)
            {
        	m_data.erase(m_data.begin()+m);
		m++;
            }
        }
}

void CProtocolMPSU::AddSystemBytes(CDataMessage* data_message)
{
    vector<BYTE> data;
    data = data_message->getPacket()->getData();
    String2Protocol(data);
    data_message->getPacket()->setData(data);    
}

void CProtocolMPSU::RemoveSystemBytes(CDataMessage* data_message)
{
    vector<BYTE> data;
    data = data_message->getPacket()->getData();
    Protocol2String(data);
    data_message->getPacket()->setData(data);    
}
void CProtocolMPSU::ClearPort()
{
    vector<BYTE> data(1);
    while( (_pdDriver->ReadData(data, 1, 0)) != -1)
    {
	INFO("Trash char was received!");
    }
}
//Check if we have read data request. If we have, it can be cached.
int CProtocolMPSU::isCacheable(vector<BYTE> data)
{
//		      D			   U
    if( (data[2] == 0x44) || (data[2] == 0x55) )
    {
	INFO("Request is cacheable.");
	return 1;
    }
    else
    {
    	INFO("Request is not cacheable.");
	return 0;
    }
}
//static
void CProtocolMPSU::update_record(pair<const vector<BYTE>, CACHE_REQUEST > request)
{
    INFO("Cache query begin.");
    if(request.second._iCounter > 0)
    {
	//port access lock
	//Make request
	//port access release
    }
    else
    {
	//delete cache record
    }    
}
void* CProtocolMPSU::run_cache(void* start)
{
    CACHE_STARTUP* startup = (CACHE_STARTUP*)start;
    CProtocol* protocol = (CProtocol*)startup->_pProtocol;
    map< vector<BYTE>, CACHE_REQUEST >*	cache = startup->_pmCache;    
    
    while(1)
    {
	for_each(cache->begin(), cache->end(), CProtocolMPSU::update_record);
	//sleep(1);
    }    
}

