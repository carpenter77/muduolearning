#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include "Declare.h"
#include "define.h"
#include "IChannelCallBack.h"
#include "channel.h"
#include "IAcceptorCallBack.h"
#include "EventLoop.h"

class Acceptor :public IChannelCallBack{
  public:
          Acceptor(EventLoop* pLoop);
          ~Acceptor();

          virtual void handleRead();
          virtual void handleWrite();
         void setCallBack(IAcceptorCallBack* pCallBack);
          void start();
  private:
          int createAndListen();
          EventLoop *_pLoop;
          int _sockfd;
          Channel* _pAcceptorChannel;
          IAcceptorCallBack* _pCallBack;
};
#endif
