// Implementation of the ServerSocket class

#include "ServerSocket.h"
#include "SocketException.h"


ServerSocket::ServerSocket ( int port )
{
  if ( ! Socket::create() )
    {
      throw SocketException ( "Could not create server socket." );
    }

  if ( ! Socket::bind ( port ) )
    {
      throw SocketException ( "Could not bind to port." );
    }

  if ( ! Socket::listen() )
    {
      throw SocketException ( "Could not listen to socket." );
    }
    
}

ServerSocket::~ServerSocket()
{
}



void ServerSocket::send( const vector<BYTE> v ) const
{
    int i = Socket::send ( v );
  if ( !i  )
    {
      throw SocketException ( "Could not write to socket." );
    }
}

int ServerSocket::recv ( vector<BYTE>& v ) 
{
    int i = Socket::recv ( v );
    
  if (i == -1)
    {
	return -1; //No message arrived
    }
  
  if ( i == 0 )
    {
      throw SocketException ( "Could not read from socket." );
      return 0; //Error     
    }
    return i; //Success
}


void ServerSocket::accept ( ServerSocket& sock )
{
  if ( ! Socket::accept ( sock ) )
    {
      throw SocketException ( "Could not accept socket." );
    }
}

void ServerSocket::set_non_blocking ( const bool b )
{
    Socket::set_non_blocking(b);
}
