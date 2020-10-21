// MessageQueue.h: interface for the CMessageQueue class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MESSAGEQUEUE_H__EF4D6A7A_07A7_492E_B4F0_B651F1A0173A__INCLUDED_)
#define AFX_MESSAGEQUEUE_H__EF4D6A7A_07A7_492E_B4F0_B651F1A0173A__INCLUDED_

#include "Semaphore.h"
#include "Message.h"

//##########################################################
class CMessageQueue
{
public:
        CMessageQueue();
        virtual ~CMessageQueue();

        void put( CMessage* msg );
        CMessage* get();
	
	//Functions for shared use of queue by multiple clients(sockets)
	//Used when allocating queues for short living nodes
	int Use();
	void Free();
	
	//Functions for operating with _sAnchor semaphore. Used for IPC between
	//router thread and protocol thread to eliminate high CPU usage.
	void Lock();
	void Unlock();
	int Empty();
	
private:

	class CComparePriorities
	{
	    public:
    	    bool operator() ( CMessage* msg1, CMessage* msg2 )
	    {
    		return msg1->getPriority() < msg2->getPriority();
	    }
	};
	
	std::priority_queue< CMessage*, vector<CMessage*>, CComparePriorities > m_queue;
	CSemaphore _sSemaphore;
	CSemaphore _sAnchor;
	int _iUsed;
};

//##########################################################
// Special interface for the client objects, which require
// having access to write the queue and do not need
// access to read one.
//##########################################################
class CMsgQueue
{
public:
        CMsgQueue( CMessageQueue* queue ) : m_queue(queue) {}
        ~CMsgQueue() {}

        void		put( CMessage* msg ) { m_queue->put(msg); }
	CMessage*	get() { return m_queue->get(); }
	
private:
        // do not call delete for this pointer!!!
        CMessageQueue* m_queue;
};

#endif // !defined(AFX_MESSAGEQUEUE_H__EF4D6A7A_07A7_492E_B4F0_B651F1A0173A__INCLUDED_)
