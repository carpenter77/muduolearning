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
#include "IAcceptorCallBack.h"
#include "Acceptor.h"
#include "TcpConnection.h"
using namespace std;

#define MAX_LINE 100
#define MAX_EVENTS 500
#define MAX_LISTENFD 5

class TcpServer:public IAcceptorCallBack{
  public:
          TcpServer();
          ~TcpServer();
          void start();
          virtual void newConnection(int sockfd);
  private:
          void update(Channel* pChannel,int op);

          int _epollfd;
          struct epoll_event _events[MAX_EVENTS];
          map<int,TcpConnection*> _connections;
          Acceptor* _pAcceptor;
};

#endif
