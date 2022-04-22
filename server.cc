#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<iostream>
#include<sys/epoll.h>
using namespace std;

#define MAX_LINE 100
#define MAX_EVENTS 500
#define MAX_LISTENFD 5
int createAndListen(){
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
int main(int args,char** argv){
  struct epoll_event ev,events[MAX_EVENTS];
  int listenfd,connfd,sockfd;
  int readlength;
  char buf[MAX_LINE];
  struct sockaddr_in clientaddr;
  socklen_t addrlen=sizeof(struct sockaddr_in);
  int epollfd=epoll_create(1);
  if(epollfd<0){
    perror("epllo_create");
    exit(1);
  }
  listenfd=createAndListen();
  ev.data.fd=listenfd;
  ev.events=EPOLLIN;
  epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&ev);

  for(;;){
    int fd=epoll_wait(epollfd,events,MAX_EVENTS,-1);
    if(fd==-1){
      perror("epoll_wait");
      exit(1);
    }
    for(int i=0;i<fd;i++){
        if(events[i].data.fd==listenfd){
          connfd=accept(listenfd,(sockaddr*)&clientaddr,(socklen_t*)&addrlen);
          if(connfd>0){
            cout<<"new connect form"<<"["<<inet_ntoa(clientaddr.sin_addr)<<" : "<<ntohs(clientaddr.sin_port)<<" ]"
                            <<" accept socket fd: "<<connfd<<endl;
          }else{
            cout<<"accept error ,connfd: "<<connfd<<" errno: "<<errno<<endl;
          }
          fcntl(connfd,F_SETFL,O_NONBLOCK); //设置非阻塞连接
          ev.data.fd=connfd;
          ev.events=EPOLLIN;
         if(-1== epoll_ctl(epollfd,EPOLL_CTL_ADD,connfd,&ev)){
            cout<<"epoll_ctl error,errno: "<<errno<<endl;
          }
      }
      else if(events[i].events & EPOLLIN){
        if((sockfd=events[i].data.fd)<0){
          cout<<"EPOLLIN sockfd"<<" error"<<endl;
          exit(1);
        }
         memset(buf,0,sizeof(buf));
         if((readlength=read(sockfd,buf,sizeof(buf)))<0){
             if(errno==ECONNRESET){
                 cout<<"ECONNREST close socket fd: "<<events[i].data.fd<<endl;
                 exit(1);
             }
         }else if(readlength==0){
             cout<<"read 0 closed socket fd: "<<events[i].data.fd<<endl;
             close(sockfd);
         }else{
             cout<<buf;
             if(write(sockfd,buf,readlength)!=readlength){
                 cout<<"error: not finished one time"<<endl;
             }
        }
      }
    }
  }
    return 0;
  }
