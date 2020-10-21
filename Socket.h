// Definition of the Socket class

#ifndef Socket_class
#define Socket_class

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "global.h"

const int MAXHOSTNAME = 200;
const int MAXCONNECTIONS = 100;
const int MAXRECV = 1000;

class Socket
{
 public:
  Socket();
  virtual ~Socket();

  // Server initialization
  bool create();
  bool bind ( const int port );
  bool listen() const;
  bool accept ( Socket& ) const;

  // Client initialization
  bool connect ( const std::string host, const int port );

  // Data Transimission
  bool send ( const vector<BYTE> ) const;
  int recv ( vector<BYTE>& ) const;


  void set_non_blocking ( const bool );

  bool is_valid() const { return m_sock != -1; }
  int get_sock() const {return m_sock;}


 private:

  int m_sock;
  sockaddr_in m_addr;


};


#endif
