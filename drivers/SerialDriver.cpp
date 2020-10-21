#include "MPSUSerialDriver.h"

CSerialDriver::CSerialDriver()
{
}

CSerialDriver::~CSerialDriver()
{
//    tcflush(_iPort_descriptor, TCIOFLUSH);
//    tcsetattr(_iPort_descriptor,TCSANOW,&_tSave_tio);
//    if(close(_iPort_descriptor) == 0)
//    cout<<"Port closed - "<<_pcPort_path<<"\n";
//    else cout<<"Error closing port - "<<_pcPort_path<<"\n";
}

int CSerialDriver::ReadData(vector<BYTE>& data, int size, int timeout)
{
//Function performs reading data from device.
//    INFO("SerialDriver::ReadData begins...");
    BYTE rdata[size];
    if( CheckTimeout(_iPort_descriptor, _iPort_descriptor, 0, timeout, 0) )
    {
	size = read(_iPort_descriptor, rdata, size);
	
        if(size == -1)
	{
    	    INFO("(in SerialDriver::WriteData) read() returns -1!");
	    return -1;
	}
	
	data = B2V(rdata, size);
//	INFO("SerialDriver::ReadData successfull...");	
	return size;    
    }
    else
    {
//	INFO("(in SerialDriver::ReadData) Timeout!!!");
	return -1;
    }
    
//Return value:    -1: Error
//                >=0: OK
}

int CSerialDriver::WriteData(const vector<BYTE>& data, int timeout)
{
//Function performs sending data to device.
//    INFO("CMPSUSerialDriver::WriteData begins...");
    int size = data.size();
    BYTE rdata[size];
    V2B(data, rdata);
    if( CheckTimeout(_iPort_descriptor, 0, _iPort_descriptor, 0, timeout) )
    {
	size = write(_iPort_descriptor, rdata, size);
    
	if(size == -1)
	{
    	    INFO("(in SerialDriver::WriteData) write() returns -1!");
	}
	
	return size;
    }
    else
    {
//	INFO("(in SerialDriver::WriteData) Timeout!!!");
	return -1;
    }
    
//Return value:    -1: Error
//                 >=0: OK
}
