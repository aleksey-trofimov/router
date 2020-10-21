// DataPacket.h: interface for the CDataPacket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAPACKET_H__862692B9_DB6C_4FFC_8E44_39A2316AB767__INCLUDED_)
#define AFX_DATAPACKET_H__862692B9_DB6C_4FFC_8E44_39A2316AB767__INCLUDED_


#include "global.h"

class CDataPacket 
{
public:
    
    CDataPacket( const vector<BYTE>& data);

    virtual ~CDataPacket(){};
    
    vector<BYTE> getData() { return m_data; }
    void setData(vector<BYTE> d) { m_data = d; }

private:
    vector<BYTE>	m_data;
};

#endif
