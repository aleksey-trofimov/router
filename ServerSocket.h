// Definition of the ServerSocket class

#ifndef ServerSocket_class
#define ServerSocket_class

#include "Socket.h"
#include "Message.h"
#include "global.h"

class ServerSocket : private Socket
{
public:

    ServerSocket ( int port );
    ServerSocket (){};
    virtual ~ServerSocket();

    void	send( const vector<BYTE>) const;
    int		recv ( vector<BYTE>&); 
    void	accept ( ServerSocket& );  
    void	set_non_blocking ( const bool );
        
    int		Get_SOCK() const {return Socket::get_sock();}
    bool 	is_valid() const { return Socket::is_valid(); }
    

private:
    
};


#endif
