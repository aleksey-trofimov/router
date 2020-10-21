// MessageQueue.cpp: implementation of the CMessageQueue class.
//
//////////////////////////////////////////////////////////////////////

#include "MessageQueue.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessageQueue::CMessageQueue()
{
    Lock();
}

CMessageQueue::~CMessageQueue()
{
}

void CMessageQueue::put( CMessage* msg )
{
    _sSemaphore.lock();
    m_queue.push( msg );
    _sSemaphore.unlock();
}

CMessage* CMessageQueue::get()
{
    _sSemaphore.lock();
	CMessage* msg;
	if( !m_queue.empty() )
	{
		msg = m_queue.top();
		m_queue.pop();
	}
	else
	{
		msg = 0;
	}
    _sSemaphore.unlock();
    return msg;
}
int CMessageQueue::Use()
{
    if(!_iUsed)
    {
    	_iUsed = 1;
	return 1;
    }
    
    return 0;
    //1 - Use successful 
    //0 - Already used 
}

void CMessageQueue::Free()
{
    //Do not use this function before Use() ;)    
    _iUsed=0;
}
void CMessageQueue::Lock()
{
    _sAnchor.lock();
}
void CMessageQueue::Unlock()
{
    _sAnchor.unlock();
}
int CMessageQueue::Empty()
{
    if(m_queue.size() == 0)
	return 1;
	
    return 0;
}
