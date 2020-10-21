// Message.cpp: implementation of the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "Message.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessage::CMessage( CAddress from, CAddress to, PRIORITY priority ) :
        m_from( from ),
        m_to( to ),
        m_priority( priority )
{
}

CMessage::~CMessage()
{
}

CDataMessage::CDataMessage( CDataPacket* data, CAddress from, CAddress to, int iProtocol, PRIORITY priority) :
        CMessage( from, to, priority ),
        m_data( data ), _iProtocol(iProtocol)
{
    vector<BYTE> copy = m_data->getData();
    m_initial_data = new CDataPacket(copy);
}
CDataMessage::CDataMessage( vector<BYTE> vdata, CAddress from, CAddress to, int iProtocol, PRIORITY priority) :
        CMessage( from, to, priority ), _iProtocol(iProtocol)
{
     m_data = new CDataPacket(vdata);
     m_initial_data = new CDataPacket(vdata);
}

CDataMessage::~CDataMessage()
{
        delete m_data;
	delete m_initial_data;
}

CCmdMessage::CCmdMessage( CDataPacket* command, CAddress from, CAddress to, PRIORITY priority ) :
        CMessage( from, to, priority ),
        m_cmd( command )
{
}

CCmdMessage::~CCmdMessage()
{
        delete m_cmd;
}
