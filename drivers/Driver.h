#if !defined DRIVER_H
#define DRIVER_H

#include "../Semaphore.h"
#include <sys/ioctl.h>
#include "../global.h"
#include "../Message.h"
#include "../MessageQueue.h"
#include "../Address.h"
#include "../system_data.h"

class CDriver
{
public:
        CDriver();
        virtual ~CDriver();


	virtual int ReadData(vector<BYTE>& data, int size, int timeout);
	virtual int WriteData(const vector<BYTE>& data, int timeout);

	int CheckTimeout(int n, int read, int write, int sec, int usec);
	//int GetSemaphore();
	//int ReleaseSemaphore();
	int GetRange(){return _iRange;}
	void SetRange(int range){_iRange = range;}
	int GetDevice(){return _iDevice;}
	void SetDevice(int number){_iDevice = number;}
	virtual int ProcessCmd(CMessage*) = 0;
	
//	virtual void start_driver() = 0;
	
private:
	//CSemaphore _sSemaphore;
	int	_iRange;//Helps to determine local address
	int	_iDevice;//port number
};

#endif
