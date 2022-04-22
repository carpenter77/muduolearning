#include "TcpServer.h"
#include<vector>

TcpServer::TcpServer()
        : _pAcceptor(NULL),_epollfd(-1){
}
TcpServer::~TcpServer()
{
}
void TcpServer::newConnection(int sockfd){
  TcpConnection* p=new TcpConnection(_epollfd,sockfd);
  _connections[sockfd]=p;
}
void TcpServer::start(){
 	 _epollfd=epoll_create(1);
  if(_epollfd<0){
    perror("epllo_create");
    exit(1);
  }
   _pAcceptor=new Acceptor(_epollfd);
  _pAcceptor->setCallBack(this);
  _pAcceptor->start();
	for(;;){
		vector<Channel*> channels;
    int fd=epoll_wait(_epollfd,_events,MAX_EVENTS,-1);
    if(fd==-1){
      perror("epoll_wait");
      exit(1);
    }
    for(int i=0;i<fd;i++){
				Channel* pChannel=static_cast<Channel*>(_events[i].data.ptr);	
				pChannel->setRevents(_events[i].events);
				channels.push_back(pChannel);
		}
		vector<Channel*>::iterator it;
		for(it=channels.begin();it!=channels.end();++it){
			(*it)->handleEvent();
		}
	}
}
