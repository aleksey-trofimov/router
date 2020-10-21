#ifndef SYSTEM_DATA
#define SYSTEM_DATA


#include "MessageQueue.h"
#include <vector>

//SYSTEM_DATA includes basic and permanent system data.
class SYS_DATA
{
public:
    //Constructor
    SYS_DATA(vector<CMessageQueue*>* pvqNodesQueues,
		CMessageQueue* pqRouter_queue,
		int iController_id)
    :_pvqNodesQueues(pvqNodesQueues),
     _pqRouter_queue(pqRouter_queue),
     _iController_id(iController_id)
    {}

    ~SYS_DATA(){}
        

    vector<CMessageQueue*>*	_pvqNodesQueues;
    CMessageQueue*		_pqRouter_queue;
    int				_iController_id;
};

#endif
