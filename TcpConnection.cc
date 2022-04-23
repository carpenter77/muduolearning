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

TcpConnection::TcpConnection(EventLoop* ploop,int sockfd)
        :_pLoop(ploop),
        _sockfd(sockfd),
        _pUser(nullptr)
{
        _pChannel=new Channel(_pLoop,_sockfd);
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
             string str(buf,MAX_LINE);
             _pUser->onMessage(this,str);
        }

}

void TcpConnection::send(const string& msg){
   int n=::write(_sockfd,msg.c_str(),msg.size());
   if( n!=static_cast<int>(msg.size()) ){
      cout<<"write error: "<<msg.size()-n<<" bytes left"<<endl;
   }
}
void TcpConnection::connectEstablished(){
  if(_pUser){
    _pUser->onConnection(this);
  }
}
void TcpConnection::setUser(IMuduoUser* user){
  _pUser=user;
}
