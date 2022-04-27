#ifndef IMUDOUUSER
#define IMUDOUUSER
#include <string>
#include "Declare.h"
#include "Buffer.h"
using namespace std;
class TcpConnection;
class IMuduoUser{
public:
  virtual void onConnection(TcpConnection* pConn)=0;
  virtual void onMessage(TcpConnection* pConn,Buffer* pBuf)=0;
  virtual void onWriteComplete(TcpConnection* pConn)=0;
};

#endif
