// CacheData.h: interface for the CacheData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined MPSURecord
#define MPSURecord

#include "../global.h"

class CMPSURecord
{
public:
    CMPSURecord(int, int, BYTE,BYTE,BYTE,BYTE,BYTE);
    CMPSURecord(int);
    CMPSURecord();
    ~CMPSURecord();
    
    int ID;
    int Device_ID;
    BYTE Ind;
    BYTE N;
    BYTE Code;
    BYTE Nchan;
    BYTE Byte;
    
    bool operator==(const CMPSURecord&);
    void print(string comment="MPSURecord dump:");
    
private:    

};
#endif
