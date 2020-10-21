// Implementation of the Socket class.


#include "Socket.h"

Socket::Socket() :
  m_sock ( -1 )
{
  memset ( &m_addr,
	   0,
	   sizeof ( m_addr ) );
}

Socket::~Socket()
{
  if ( is_valid() )
  {
    ::shutdown(m_sock,2);
    ::close ( m_sock );
  }
}

bool Socket::create()
{
  m_sock = socket ( AF_INET,
		    SOCK_STREAM,
		    0 );

  if ( ! is_valid() )
    return false;


  // TIME_WAIT - argh
  int on = 1;
  if ( setsockopt ( m_sock, SOL_SOCKET, SO_REUSEADDR, ( const char* ) &on, sizeof ( on ) ) == -1 )
    return false;


  return true;

}



bool Socket::bind ( const int port )
{

  if ( ! is_valid() )
    {
      return false;
    }
    
  m_addr.sin_family = AF_INET;
  m_addr.sin_addr.s_addr = INADDR_ANY;
  m_addr.sin_port = htons ( port );

  int bind_return = ::bind ( m_sock,
			     ( struct sockaddr * ) &m_addr,
			     sizeof ( m_addr ) );
			     
  if ( bind_return == -1 )
    {
      return false;
    }

  return true;
}


bool Socket::listen() const
{
  if ( ! is_valid() )
    {
      return false;
    }

  int listen_return = ::listen ( m_sock, MAXCONNECTIONS );

  if ( listen_return == -1 )
    {
      return false;
    }

  return true;
}


bool Socket::accept ( Socket& new_socket ) const
{
  int addr_length = sizeof ( m_addr );
  new_socket.m_sock = ::accept ( m_sock, ( sockaddr * ) &m_addr, ( socklen_t * ) &addr_length );

  if ( new_socket.m_sock <= 0 )
    return false;
  else
    return true;
}


bool Socket::send (const vector<BYTE> v ) const
{
            cout<<"Attemption to send\n";
    int count = 0;
    long vsize = v.size();
    int bsize = 0;
    struct timeval st_delay;
    fd_set write_set;
    FD_ZERO( &write_set );
    FD_SET ( m_sock, &write_set );

    
    while(count < vsize)
    {
	if((vsize-count) <= 1460)
	    bsize = vsize-count;
	else
	    bsize = 1460;
	
	char buf[bsize];
    
	for(int i = 0; i< bsize; i++)
	{
	    buf[i] = v[count++];
	}  
	
	st_delay.tv_sec = 60;
	st_delay.tv_usec = 0;
	if(select(m_sock+1, NULL, &write_set, NULL, &st_delay) == 0)
	    {
		LOG_IT("Send failed, TIMEOUT!\n");
		return false;
	    }

	int status = ::send ( m_sock, buf, sizeof(buf), MSG_NOSIGNAL );
	if ( status == -1 )
	{
	    LOG_IT("Send failed\n");
	    return false;
	}
	bsize = 0;
    }
    return true;
}


int Socket::recv ( vector<BYTE>& v ) const
{

  char buf [ MAXRECV + 1 ];

  v.erase(v.begin(), v.end());

  memset ( buf, 0, MAXRECV + 1 );

  int status = ::recv ( m_sock, buf, MAXRECV, 0 );

  if ( (status == -1) && (errno == EAGAIN) )
    {
      return -1;
    }
  if ( status == -1 )
    {
      LOG_IT("status == -1 in Socket::recv\n");
      return 0;
    }
  else if ( status == 0 )
    {
      LOG_IT("status == 0 in Socket::recv\n");
      return 0;
    }
  else
    {
	for(int i = 0; i< status; i++)
	{
	    v.push_back(buf[i]);
	}
      return status;
    }
}

bool Socket::connect ( const std::string host, const int port )
{
        cout<<"Attemption to connect\n";
  if ( ! is_valid() ) return false;

  m_addr.sin_family = AF_INET;
  m_addr.sin_port = htons ( port );

  int status = inet_pton ( AF_INET, host.c_str(), &m_addr.sin_addr );

  if ( errno == EAFNOSUPPORT ) return false;

  status = ::connect ( m_sock, ( sockaddr * ) &m_addr, sizeof ( m_addr ) );

  if ( status == 0 )
    return true;
  else
    return false;
}

void Socket::set_non_blocking ( const bool b )
{

  int opts;

  opts = fcntl ( m_sock,
		 F_GETFL );

  if ( opts < 0 )
    {
      return;
    }

  if ( b )
    opts = ( opts | O_NONBLOCK );
  else
    opts = ( opts & ~O_NONBLOCK );

  fcntl ( m_sock,
	  F_SETFL,opts );

}
