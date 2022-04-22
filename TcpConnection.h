#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H
#include "Declare.h"
#include "IChannelCallBack.h"
#include "channel.h"
class TcpConnection: public IChannelCallBack{
  public:
          TcpConnection(int epollfd,int sockfd);
          ~TcpConnection();
          virtual void OnIn(int sockfd);
  private:
        int _epollfd;
        int _sockfd;
        Channel* _pChannel;
};

#endif
