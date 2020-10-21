#if !defined MPSUSerialDriver
#define MPSUSerialDriver

#include "SerialDriver.h"
#include <termios.h>

#define MPSUBAUDRATE B9600

class CMPSUSerialDriver : public CSerialDriver
{
public:
        CMPSUSerialDriver(int port);
        ~CMPSUSerialDriver();
	
	void InitPort();
	
	int ProcessCmd(CMessage*){}
};

class CMOUSESerialDriver : public CSerialDriver
{
public:
        CMOUSESerialDriver(int port);
        ~CMOUSESerialDriver();
	
	void InitPort();
	
	int ProcessCmd(CMessage*){}
};

class CCONFIGSerialDriver : public CSerialDriver
{
public:
        CCONFIGSerialDriver(int port);
        ~CCONFIGSerialDriver();
	
	void InitPort();
	
	int ProcessCmd(CMessage*){}
};



#endif
