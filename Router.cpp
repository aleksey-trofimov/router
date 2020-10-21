#include "Router.h"

CRouter::CRouter(CMessageQueue*	pmqRouter_queue, vector<CMessageQueue*>* pvqNodesQueues)
:_pmqRouter_queue(pmqRouter_queue), _pvqNodesQueues(pvqNodesQueues)
{
}

CRouter::~CRouter()
{
}
void CRouter::start()
{
    //Here we can do any additional operations before running router thread

    //Run router thread
    pthread_t router_thread;
    pthread_create( &router_thread, 0, run_thread, (void*)this);
    pthread_detach(router_thread);    
}

//static
void* CRouter::run_thread(void* rout)
{
    INFO("Router thread started");
    CRouter* router = (CRouter*)rout;
    CDataMessage*	data_message;
    CMessage*		message;
    CCmdMessage*	cmd_message;
    CAddress		destination;
    //////////////////////
    struct timeval tv;
    //////////////////////
    //Wait for messages in router queue
    while(1)
    {
	INFO("ROUTER: Waiting for unlock..");
	router->GetRouterQueue()->Lock();
	INFO("ROUTER: Queue locked!");
	do
	{	    
	    //Wait
	    tv.tv_sec = 0;
	    tv.tv_usec = 1000;
	    select( 1, NULL, NULL, NULL, &tv );
	    //Get message from node queue
	    INFO("ROUTER: before getting message from queue");
	    message = router->GetRouterQueue()->get();
	    INFO("ROUTER: message was received by router");
	    if(message != 0)
	    {
	    destination = message->getTo();
	    //Put message to destination node queue without any address check
	    //In future must add address check to give answers for nodes
	    //that send messages to invalid nodes	    
	    
	    //Check existance of destination node.
	    if( router->GetNodesQueues()->operator[](destination.GetLAddress())->Use() )
	    {	    
		INFO("ROUTER: attempt to send to invalid node");
		router->GetNodesQueues()->operator[](destination.GetLAddress())->Free();
		delete message;
	    }
	    else
	    {
		router->SendToNode(message, destination);		
		INFO("ROUTER: message was sent to node");
	    }
	    }
	    else
	    {
		INFO("ERROR! Zero message pointer");
	    }
	    
	}while(!(router->GetRouterQueue()->Empty()));
    }    
}

int CRouter::SendToNode(CMessage* message, CAddress destination)
{
     GetNodesQueues()->operator[](destination.GetLAddress())->put(message);
     GetNodesQueues()->operator[](destination.GetLAddress())->Unlock();
}
