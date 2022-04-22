#include<errno.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>

#include "TcpConnection.h"
#include "channel.h"
#include "define.h"
#include<string.h>
#include<iostream>
using namespace std;

TcpConnection::TcpConnection(int epollfd,int sockfd)
        :_epollfd(epollfd),
        _sockfd(sockfd)
{
        _pChannel=new Channel(_epollfd,_sockfd);
        _pChannel->setCallBack(this);
        _pChannel->enableReading();
}
TcpConnection::~TcpConnection(){}
void TcpConnection::OnIn(int sockfd){
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
