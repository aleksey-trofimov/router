#include "Driver.h"

CDriver::CDriver()
{
}

CDriver::~CDriver()
{
}

int CDriver::ReadData(vector<BYTE>& data, int size, int timeout)
{
    INFO("CDriver::ReadData executed! Must be a error!");
    return -1;
//Return value:    -1: Error
}

int CDriver::WriteData(const vector<BYTE>& data, int timeout)
{
    INFO("CDriver::WriteData executed! Must be a error!");
    return -1;
//Return value:    -1: Error
}
int CDriver::CheckTimeout(int n, int read, int write, int sec, int usec)
{
    fd_set read_set;
    fd_set write_set;
    int retval;
    
    struct timeval tv;
    tv.tv_sec = sec;
    tv.tv_usec = usec;

    FD_ZERO( &read_set );
    FD_ZERO( &write_set );

    FD_SET ( read, &read_set );
    FD_SET ( write, &write_set );
    
    if(read)
	retval = select( n+1, &read_set, NULL, NULL, &tv );
	
    if(write)
	retval = select( n+1, NULL, &write_set, NULL, &tv );
    
    return retval;
//Return value:    -1: Error
//                  0: Timeout 
//                 >0: OK
}

/*int CDriver::GetSemaphore()
{
    _sSemaphore.lock();
}

int CDriver::ReleaseSemaphore()
{
    _sSemaphore.unlock();
}
*/
