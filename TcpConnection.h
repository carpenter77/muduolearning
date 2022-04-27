#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include "Declare.h"
#include "IChannelCallBack.h"
#include "channel.h"
#include "EventLoop.h"
#include "IAcceptorCallBack.h"
#include <string>
#include "IRun.h"
#include "IMuduoUser.h"
using namespace std;
class TcpConnection: public IChannelCallBack,IRun{
  public:
          TcpConnection(EventLoop* pLoop,int sockfd);
          ~TcpConnection();
          void setCallback(IAcceptorCallBack *pCallBack);
          void send(const string& message);
          void connectEstablished();
          void setUser(IMuduoUser* puser);

          virtual void handleRead();
          virtual void handleWrite();
          virtual void run();
  private:
        EventLoop *_pLoop;
        IMuduoUser* _pUser;
        int _sockfd;
        Channel* _pChannel;
        Buffer _inBuf;
        Buffer _outBuf;
};

#endif
