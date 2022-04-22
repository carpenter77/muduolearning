#include "TcpServer.h"
#include<vector>

TcpServer::TcpServer()
        : _sockfd(-1),_epollfd(-1){

}
TcpServer::~TcpServer()
{

}
int TcpServer::createAndListen(){
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
void TcpServer::OnIn(int sockfd){
    cout<<"OnIn: "<<sockfd<<endl;
    if(sockfd==_sockfd){
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
					Channel *pChannel=new Channel(_epollfd,connfd);
					pChannel->setCallBack(this);
					pChannel->enableReading();
     }else{
        int readlength;
				char buf[MAX_LINE];
				if(sockfd<0){
          cout<<"EPOLLIN sockfd"<<" error"<<endl;
          exit(1);
        }
         memset(buf,0,sizeof(buf));
         if((readlength=read(sockfd,buf,sizeof(buf)))<0){
             if(errno==ECONNRESET){
                 cout<<"ECONNREST close socket fd: "<<sockfd<<endl;
                 exit(1);
             }
         }else if(readlength==0){
             cout<<"read 0 closed socket fd: "<<sockfd<<endl;
             close(sockfd);
         }else{
             cout<<buf;
             if(write(sockfd,buf,readlength)!=readlength){
                 cout<<"error: not finished one time"<<endl;
             }
				}
		} 
}
void TcpServer::start(){
 	 _epollfd=epoll_create(1);
  if(_epollfd<0){
    perror("epllo_create");
    exit(1);
  }
  _sockfd=createAndListen(); 
  Channel* pChannel=new Channel(_epollfd,_sockfd);
  pChannel->setCallBack(this);
  pChannel->enableReading();
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
