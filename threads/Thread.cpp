#include "Thread.h"

CThread::CThread()
{
}

CThread::~CThread()
{
}

CProtocol* CThread::CreateProtocol(CDriver* driver)
{
//Get node number
////////////////////////////////////////////////////////////////////////////////
    int node=-1;
    switch( driver->GetRange() )
    {
        case 1:
	{
	    //Serial port 1..32
	    int device = driver->GetDevice();
	    //Check if port was already allocated
	    if(_data._psSysData->_pvqNodesQueues->operator[](device+1)->Use())
		node = 1+device;
	    else
		INFO("Error! Unable to Use() Node for serial port.")
	}
        break;
	
	case 2:
	{
	    //Parallel port 33..64
	    int device = driver->GetDevice();
	    //Check if port was already allocated
	    if(_data._psSysData->_pvqNodesQueues->operator[](device+33)->Use())
		node = 33+device;
	    else
		INFO("Error! Unable to Use() Node for parallel port.")
	}
        break;
	
	case 3:
	{
	    //Ethernet 65..164
	    //Find first free node
	    for(int i=65; i<165;i++)
	    {
		if(_data._psSysData->_pvqNodesQueues->operator[](i)->Use())
		{
		    node = i;
		    i=165;
		}
	    }
	}
        break;
	    
	default:
	{
	    INFO("Error! Unknown range!")
	}
    }    
//node now contains number of local node for new protocol object
    
    CAddress aNodeAddress(_data._psSysData->_iController_id, node);
    
    switch( _data._iProtocolCode )
    {
        case PROTOCOL_CONFIG:
	{
	    CProtocolCONFIG* protocol = new CProtocolCONFIG(
				_data._psSysData->_pqRouter_queue,
				_data._psSysData->_pvqNodesQueues->operator[](node),
				driver,
				aNodeAddress);
	    return protocol;
	}
	break;

        case PROTOCOL_MPSU:
	{
	    CProtocolMPSU* protocol = new CProtocolMPSU(
				_data._psSysData->_pqRouter_queue,
				_data._psSysData->_pvqNodesQueues->operator[](node),
				driver,
				aNodeAddress);
	    return protocol;
	}
	break;
	
	case PROTOCOL_MOUSE:
	{
	    CProtocolMOUSE* protocol = new CProtocolMOUSE(
				_data._psSysData->_pqRouter_queue,
				_data._psSysData->_pvqNodesQueues->operator[](node),
				driver,
				aNodeAddress);
	    return protocol;
	}
	break;

	
        case PROTOCOL_MODBUS_TCP:
	{
	    CMODBUS_TCP* protocol = new CMODBUS_TCP(
					    _data._psSysData->_pqRouter_queue,
					    _data._psSysData->_pvqNodesQueues->operator[](node),
					    driver,
					    aNodeAddress);
	    return protocol;
	}
	break;
	
	default:
	{
	    //Unknown protocol. Nothing can be done for now.	    
	    return 0;
	}	
    }
}
