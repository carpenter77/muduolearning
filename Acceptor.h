#ifndef ACCEPTOR_H
#define ACCEPTOR_H
#include "Declare.h"
#include "define.h"
#include "IChannelCallBack.h"
#include "channel.h"
#include "IAcceptorCallBack.h"

class Acceptor :public IChannelCallBack{
  public:
          Acceptor(int epollfd);
          ~Acceptor();

          virtual void OnIn(int socket);
          void setCallBack(IAcceptorCallBack* pCallBack);
          void start();
  private:
          int createAndListen();
          int _epollfd;
          int _sockfd;
          Channel* _pAcceptorChannel;
          IAcceptorCallBack* _pCallBack;
};
#endif
