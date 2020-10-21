// Message.h: interface for the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGE_H__B6DD83BA_1EA1_4F36_93B1_FB174EE977BC__INCLUDED_)
#define AFX_MESSAGE_H__B6DD83BA_1EA1_4F36_93B1_FB174EE977BC__INCLUDED_

#include "global.h"

#include "DataPacket.h"
#include "CmdPacket.h"
#include "Address.h"

//##########################################################
// Basic message class describes common message interface.
// This class has virtual destructor to avoid creation
// of the unspecified message objects. You should use
// CDataMessage and CCmdMessage classes for specific
// message creation.
//##########################################################

class CMessage
{
public:
	CMessage( CAddress from, CAddress to, PRIORITY priority );
	CMessage()
	{
	    m_from = CAddress(0,0);
	    m_to = CAddress(0,0);
	    m_priority = 0;
	}
        virtual ~CMessage() = 0;

        CAddress	getFrom()     { return m_from; }
        CAddress	getTo()       { return m_to; }
        PRIORITY	getPriority() { return m_priority; }
	
	void		setFrom(CAddress from_)	{ m_from = from_; }
        void		setTo(CAddress to_)	{ m_to = to_; }
	void		setPriority(PRIORITY pr_) { m_priority = pr_; }

protected:
        CAddress	m_from;
        CAddress	m_to;
        PRIORITY	m_priority;
};

//##########################################################
// Data message contains data filed, which holds passed
// data packet.
//##########################################################
class CDataMessage : public CMessage
{
public:
        CDataMessage( CDataPacket* data, CAddress from, CAddress to, int protocol, PRIORITY priority=0);
	CDataMessage( vector<BYTE> vdata, CAddress from, CAddress to, int protocol, PRIORITY priority=0);
	~CDataMessage();

        CDataPacket* getPacket(void) {return m_data;}
	CDataPacket* getInitialPacket(void) {return m_initial_data;}
	int getProtocol(void) {return _iProtocol;}
	void setProtocol(int code){_iProtocol = code;}
	
	int isCacheable(){return _iCacheable;}
	void setCacheable(int i){_iCacheable = i;}

private:
        CDataPacket* m_data;
	//Data that was initially placed into DataMessage. We need this for
	//easy MPSU 2 MODBUS_TCP conversion
	CDataPacket* m_initial_data;
	//Protocol number to determine protocol of m_data.
	int _iProtocol;	
	int _iCacheable;
};

//##########################################################
// Command message contains command assigned to the
// specified channel driver.
//##########################################################
class CCmdMessage : public CMessage
{
public:
        CCmdMessage( CDataPacket* command, CAddress from, CAddress to, PRIORITY priority=0 );
	~CCmdMessage();

        CDataPacket* getCommand(void) { return m_cmd; }

private:
	CDataPacket* m_cmd;
};

#endif // !defined(AFX_MESSAGE_H__B6DD83BA_1EA1_4F36_93B1_FB174EE977BC__INCLUDED_)
