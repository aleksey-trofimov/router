//This is the common serial port driver functions.
#if !defined SerialDriver
#define SerialDriver

#include "Driver.h"
#include <termios.h>

class CSerialDriver : public CDriver
{
public:
        CSerialDriver();
        virtual ~CSerialDriver();
	
	virtual void InitPort() = 0;
	
	virtual int ProcessCmd(CMessage*){}

	int ReadData(vector<BYTE>& data, int size, int timeout);
        int WriteData(const vector<BYTE>& data, int timeout);
	
	
	int 		_iPort_descriptor;
	struct termios 	_tSave_tio; //contains old COM port settings
	char		_pcPort_path[20]; //contains path to port device file
};

#endif
