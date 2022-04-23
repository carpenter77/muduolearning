#ifndef IMUDOUUSER
#define IMUDOUUSER
#include <string>
#include "Declare.h"
using namespace std;
class TcpConnection;
class IMuduoUser{
public:
  virtual void onConnection(TcpConnection* pConn)=0;
  virtual void onMessage(TcpConnection* pConn,const string & msg)=0;
};

#endif
