#if !defined Router
#define Router

#include "global.h"
#include "MessageQueue.h"
#include "Message.h"
#include "Address.h"


class CRouter
{
public:
        CRouter(CMessageQueue*	pmqRouter_queue, vector<CMessageQueue*>* pvqNodesQueues);
        ~CRouter();	
	
	void start();
	static void* run_thread(void* rout);

	int SendToNode(CMessage* message, CAddress destination);
	
	vector<CMessageQueue*>* GetNodesQueues(){return _pvqNodesQueues;}
	CMessageQueue* GetRouterQueue(){return _pmqRouter_queue;}
	
private:
	CMessageQueue*		_pmqRouter_queue;
	vector<CMessageQueue*>* _pvqNodesQueues;
};

#endif
