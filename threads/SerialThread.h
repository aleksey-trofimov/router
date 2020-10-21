#if !defined SerialThread_h
#define SerialThread_h

#include "../ServerSocket.h"
#include "../protocols/Protocol.h"
#include "../drivers/Driver.h"
#include "../system_data.h"
#include "Thread.h"

class CSerialThread : public CThread
{
public:
        CSerialThread(SYS_DATA* psSysData, int iProtocolCode, int port);
        ~CSerialThread();
};
#endif
