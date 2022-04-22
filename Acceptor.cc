#include "Acceptor.h"
#include "channel.h"
#include<sys/socket.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>
#include "IAcceptorCallBack.h"
#include<iostream>
using namespace std;

Acceptor::Acceptor(int epollfd)
        :_epollfd(epollfd),
        _sockfd(-1),
        _pAcceptorChannel(nullptr),
        _pCallBack(nullptr)

{
}
Acceptor::~Acceptor(){}
void Acceptor::start()
{
    _sockfd= createAndListen();
    _pAcceptorChannel= new Channel(_epollfd, _sockfd); // Memory Leak !!!
    _pAcceptorChannel->setCallBack(this);
    _pAcceptorChannel->enableReading();
}
int Acceptor::createAndListen(){
	int fd=socket(AF_INET,SOCK_STREAM,0);
  int on=1;
  struct sockaddr_in servAddr;
  fcntl(fd,F_SETFL,O_NONBLOCK);//非阻塞IO
  setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  servAddr.sin_family=AF_INET;
  servAddr.sin_addr.s_addr=htonl(INADDR_ANY);
  servAddr.sin_port=htons(9000);
  if((bind(fd,(struct sockaddr*)&servAddr,sizeof(servAddr)))<0){
      perror("bind error");
      exit(1);
  }
  if((listen(fd,MAX_LISTENFD))<0){
    perror("bind error");
      exit(1);
  }
  return fd;
}
void Acceptor::OnIn(int socket){
			int connfd;
          struct sockaddr_in clientaddr;
          socklen_t addrlen=sizeof(struct sockaddr_in);
          connfd=accept(_sockfd,(sockaddr*)&clientaddr,(socklen_t*)&addrlen);
          if(connfd>0){
            cout<<"new connect form"<<"["<<inet_ntoa(clientaddr.sin_addr)<<" : "<<ntohs(clientaddr.sin_port)<<" ]"
                            <<" accept socket fd: "<<connfd<<endl;
          }else{
            cout<<"accept error ,connfd: "<<connfd<<" errno: "<<errno<<endl;
          }
          fcntl(connfd,F_SETFL,O_NONBLOCK); //设置非阻塞连接
		_pCallBack->newConnection(connfd);
}
void Acceptor::setCallBack(IAcceptorCallBack* pCallBack){
	_pCallBack=pCallBack;
}
