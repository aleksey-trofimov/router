#include "Protocol.h"

CProtocol::CProtocol(	CMessageQueue* Router_queue,
			CMessageQueue* Node_queue,
			CDriver* pDriver,
			CAddress aAddress)
:_pmqRouter_queue(Router_queue),
 _pmqNode_queue(Node_queue),
 _pdDriver(pDriver),
 _aAddress(aAddress)
{
}

CProtocol::~CProtocol()
{
    _pmqNode_queue->Free();
    delete _pdDriver;    
}

CMessage* CProtocol::OutNodeQueue()
{
    CMessage* msg = _pmqNode_queue->get();
    return msg;
}

void CProtocol::InRouterQueue(CMessage* msg)
{    
    _pmqRouter_queue->put(msg);
}

CMessageQueue* CProtocol::GetNodeQueue()
{
    return _pmqNode_queue;
}

CMessageQueue* CProtocol::GetRouterQueue()
{
    return _pmqRouter_queue;
}

/////////////////////////////Protocol conversions///////////////////////////////

int CProtocol::Convert(CDataMessage* data_message, int protocol)
{
    vector<BYTE> data;
    data = data_message->getPacket()->getData();
    
    int iSourceProtocol = data_message->getProtocol();
    int iTargetProtocol = _iProtocol;
    
    //Return 1 if protocols are equal
    if(iSourceProtocol == iTargetProtocol)
    {
	INFO("Protocols are equal. This must be cached request conversion attempt.");
	return 1;
    }
	
    if((iSourceProtocol == PROTOCOL_MODBUS_TCP) && (iTargetProtocol == PROTOCOL_MPSU))
    {
	//Convert PROTOCOL_MODBUS_TCP into PROTOCOL_MPSU	
	MODBUS_TCP2MPSU(data);
	data_message->setProtocol(PROTOCOL_MPSU);
	data_message->getPacket()->setData(data);
	return 1;
    }
            
    if((iSourceProtocol == PROTOCOL_MPSU) && (iTargetProtocol == PROTOCOL_MODBUS_TCP))
    {
	//Convert PROTOCOL_MODBUS_TCP into PROTOCOL_MPSU	
	MPSU2MODBUS_TCP(data, data_message->getInitialPacket()->getData());
	data_message->setProtocol(PROTOCOL_MODBUS_TCP);
	data_message->getPacket()->setData(data);
	return 1;
    }
    
    else
    {
	//Unknown protocol conversion required
	INFO("Unknown protocol conversion");
	return 0;	
    }
}

///////////////////////MODBUS 2 MPSU Conversion part////////////////////////////

int CProtocol::MODBUS_TCP2MPSU(vector<BYTE>& modbus_data)
{
    vector<BYTE> mpsu_data;
	
    //MODBUS operation code	
    int MODBUS_code = modbus_data[7];
    //ID of specific module in MPSU
    int MODULE_id = modbus_data[8]*256 + modbus_data[9];
    //ID of specific MPSU
    int MPSU_id = modbus_data[6];    
    
    CMPSURecord MPSU_fields(MODULE_id);

    //We must create different MPSU data according to MODBUS operation code	
    switch( MODBUS_code )
    {
	case 3:
	{
	    //Read data command conversion
	    mpsu_data.push_back(MPSU_fields.Ind);
	    mpsu_data.push_back(MPSU_fields.N);
	    mpsu_data.push_back(MPSU_fields.Code);
	    mpsu_data.push_back(MPSU_fields.Nchan);
	}
	break;
		
	case 6:
	{
	    //Write data command conversion
	    mpsu_data.push_back(MPSU_fields.Ind);
	    mpsu_data.push_back(MPSU_fields.N);
	    mpsu_data.push_back(MPSU_fields.Code);
	    mpsu_data.push_back(MPSU_fields.Nchan);
	    
	    if(MPSU_fields.Ind == 11)
	    {
		//Analog M210 processing
		INFO("M210 processing...");
		short digit = modbus_data[11] + modbus_data[10]*256;
		short analog = Digit2Analog(digit);
	
		mpsu_data.push_back( (BYTE)(analog & 0xFF) );
		mpsu_data.push_back( (BYTE)((analog>>8) & 0xFF) );	
	    }
	    if(MPSU_fields.Ind == 5)
	    {
		//Digital M203 processing
		INFO("M203 processing...");
		mpsu_data.push_back(modbus_data[11]);
		mpsu_data.push_back(modbus_data[10]);
	    }
	}
	break;
	
	default:
	{
	    INFO("WARNING! Unsupported code");
	    //TODO: must process this situation. For example, create
	    //      error MODBUS message.
	    return 0;
	}
	break;
    }
    modbus_data = mpsu_data;
    INFO("MODBUS_TCP2MPSU executed");
    return 1;
}
short CProtocol::Analog2Digit(short analog)
{
    //This function converts MPSU
    //analog : 0 1000 100|10111001
    short output;    
    
    //Get sign:
    int sign = analog & 0x8000; //1000 0000 00000000
    
    //Get mantissa
    int mantissa = analog & 0x07FF;
    
    if(sign)
	output = ANALOG_ZERO - mantissa;
    else
	output = ANALOG_ZERO + mantissa;
	
    return output;        
}

short CProtocol::Digit2Analog(short digit)
{
    short output = 0xFFFF; //1 1111 111|11111111    
    
    if((digit<0) || (digit>4096))
	return 0;//Range error
    
    //Set sign:
    if((digit>=2048) && (digit<=4096))
    {
	//+
	output = 0xFFFF / 2;
	digit = digit-2048;
    } 
    else if(digit>=0)
	//-
	digit = 2048-digit;
        
    //Here we have 0 1111 111|11111111
    
    //Set 4 bytes to 1 0 0 0
    output = output & 0xC7FF;
    //Here we have 0 1000 111|11111111    
    
    //Set mantissa to given number    
    //0 0000 100|10111001  
    digit = digit + 0xF800;
    
	     //0 1000 111|11111111  
		      //1 1111 100|10111001    
    output = output & digit;
    //0 1000 100|10111001
    return output;    
}

///////////////////////MPSU 2 MODBUS Conversion part////////////////////////////
//!!! For now, only MPSU answer can be converted to MODBUS_TCP
//In this function we must replace data in MPSU_answer to MODBUS data
int CProtocol::MPSU2MODBUS_TCP(vector<BYTE>& MPSU_answer, vector<BYTE> modbus_request)
{
    vector<BYTE> answer = modbus_request;
    int error = 0;
    //We must determine what kind of MODBUS request was initially received
    //and act according to the function code
    int modbus_operation_code = modbus_request[7];
    //Create MPSU_fields object using received ID
    int MODULE_id = modbus_request[8]*256 + modbus_request[9];
    CMPSURecord MPSU_fields(MODULE_id);    
    
    //"Read data" request was received
    if(modbus_operation_code == 3)
    {
	INFO("Entering 3 code (read request) answer section");
        switch( MPSU_answer[0] )
        {
    	    case 1:
	    {
		INFO("OK. Creating MODBUS answer");
		answer.erase(answer.begin()+8,
		    	    answer.end());
    		answer[5] = 5;
		answer.push_back(2);//Answer length
		
		if(MPSU_fields.Ind == 6)
		{
		    //Analog M204 processing
		    INFO("M204 processing...");
		    short analog = MPSU_answer[4+(MPSU_fields.Byte*2)+1]*256
		                  +MPSU_answer[4+(MPSU_fields.Byte*2)];
//		    cout<<"analog counted:"<<analog<<"\n";
		    short digit = Analog2Digit(analog);
	
		    answer.push_back( (BYTE)((digit>>8) & 0xFF) );
		    answer.push_back( (BYTE)(digit & 0xFF) );
		}
		
		if(MPSU_fields.Ind == 3)
		{
		    //Digital M201 processing
		    INFO("M201 processing...");
		    answer.push_back(MPSU_answer[4+(MPSU_fields.Byte*2) + 1]);//1st byte of answer
    		    answer.push_back(MPSU_answer[4+(MPSU_fields.Byte*2)]);//2nd byte of answer	
		}

		MPSU_answer = answer;
		TEXTOUT("OK! Converted to MODBUS protocol:", MPSU_answer);
		INFO("MPSU2MODBUS_TCP executed");
		return 1;
	    }
	    break;
	    
	    default:
	    {
		if(MPSU_answer[0] == 2)
		{
		    INFO("Answer reporting error. Creating error message..");
		    error = 1;
		}
		else
		{
		    cout<<"Unknown MPSU answer status code:"<<(int)MPSU_answer[0]<<"\n";
		    error = 1;
		}
	    }
	}
    }
    if(error)
    {
    	answer.erase(answer.begin()+7, answer.end());
	answer[5] = 3;
    	answer.push_back(ERROR_CODE_3);
    	answer.push_back(1);
	
	MPSU_answer = answer;
	TEXTOUT("Error! Converted to MODBUS protocol:", MPSU_answer);
	
	return 0;
    }
    
    //"Write data" request was received
    if(modbus_operation_code == 6)
    {
	INFO("Entering 6 code (write request) answer section");
	switch( MPSU_answer[0] )
        {
    	    case 1:
	    {
	        INFO("OK. Sending back original message");
		MPSU_answer = modbus_request;
		return 1;
	    }
	    break;
	    
	    default:
	    {
		if(MPSU_answer[0] == 2)
		{
		    INFO("Answer reporting error. Creating error message");
		    error = 1;
		}
		else
		{
		    cout<<"Unknown MPSU answer status code:"<< (int)MPSU_answer[0]<<"\n";
		    error = 1;
		}
	    }
	}
    }
    if(error)
    {
    	answer.erase(answer.begin()+7, answer.end());
    	answer[5] = 3;
    	answer.push_back(ERROR_CODE_6);
	answer.push_back(1);
	
	MPSU_answer = answer;
	TEXTOUT("Error! Converted to MODBUS protocol:", MPSU_answer);
	
	return 0;
    }    
    INFO("Error! We must never reach this point!!!");
    return 0;    
}
