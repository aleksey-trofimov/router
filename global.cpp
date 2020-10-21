// Definition of the logging function

#include "global.h"

extern void LOG_IT(std::string info)
{
    openlog("router", LOG_PID|LOG_CONS,LOG_USER);
    syslog(LOG_DEBUG, info.c_str());
    closelog();
}

extern void TEXTOUT(std::string info, vector<BYTE> v)
{
#ifdef DEBUG_MODE

    cout<<info;
    for(int i=0; i< v.size(); i++)
	cout<<(int)v[i]<<" ";
	
    cout<<"\n";    
    
#endif
}

extern void TEXTOUT(std::string info)
{
#ifdef DEBUG_MODE
    cout<<info<<"\n";
#endif
}

extern void TEXTOUT(std::string info, BYTE number)
{
#ifdef DEBUG_MODE
    cout<<info<<number<<"\n";
#endif
}

extern vector<BYTE> ERROR()
{
    std::string sssss = "ERROR";
    vector<BYTE> error;
    for(int ii=0; ii<sssss.size(); ii++)
	error.push_back(sssss[ii]);
	
    return error;
}

extern vector<BYTE> B2V(BYTE* data, int size)
{
    vector<BYTE> vdata;
    for(int i=0; i<size; i++)
	vdata.push_back(data[i]);
	
    return vdata;
}

extern void V2B(vector<BYTE> vdata, BYTE* output)
{
    int size=vdata.size();
    
    for(int i=0; i<size; i++)
	output[i] = vdata[i];    
}
