#ifndef ECHOSERVER
#define ECHOSERVER

#include "IMuduoUser.h"
#include "TcpServer.h"
#include "Declare.h"
#include "EventLoop.h"
#include "Buffer.h"
class EchoServer: public IMuduoUser{
  private:
          TcpServer _pServer;
          EventLoop* _loop;
  public:
          EchoServer(EventLoop* loop);
          ~EchoServer();
          void start();
          virtual void onConnection(TcpConnection* pConn);
          virtual void onMessage(TcpConnection* pConn,Buffer*);
          virtual void onWriteComplete(TcpConnection* pCon);
};
#endif
