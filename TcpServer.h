#ifndef TCPSERVER_H
#define TCPSERVER_H

#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<sys/epoll.h>
#include<map>
#include "Declare.h"
#include "define.h"
#include "channel.h"
using namespace std;

#define MAX_LINE 100
#define MAX_EVENTS 500
#define MAX_LISTENFD 5

class TcpServer:public IChannelCallBack{
  public:
          TcpServer();
          ~TcpServer();
          void start();
          virtual void OnIn(int sockfd);
  private:
          int createAndListen();
          void update(Channel* pChannel,int op);

          int _epollfd;
          int _sockfd;
          struct epoll_event _events[MAX_EVENTS];
          map<int,Channel*> _channels;
};

#endif
