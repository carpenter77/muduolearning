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
void TcpConnection::handleRead(){
				int readlength;
        int sockfd=_pChannel->getSockfd();
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
             string str(buf,readlength);
             _inBuf.append(str);
             _pUser->onMessage(this,&_inBuf);
        }

}

void TcpConnection::handleWrite(){
   int sockfd=_pChannel->getSockfd();
   //临时数据
   if (_pChannel->isWriting()){
    int n=::write(_sockfd,_outBuf.peek(),_outBuf.readableBytes());
    if( n>0 ){
      cout<<"write : "<<n<<"bytes"<<endl;
      _outBuf.retrieve(n);
      if(_outBuf.readableBytes()==0){
          _pChannel->disableWriting();
          _pLoop->queueLoop(this);// write complete
      }
    }

   }
}
void TcpConnection::send(const string& msg){
   int n;
   if (_outBuf.readableBytes()==0){//不存在上轮没发完的数据
      n=::write(_sockfd,msg.c_str(),msg.size());
      if( n<0 ){
        cout<<"send write error: "<<endl;
      }
      if(n== static_cast<int>(msg.size())){
        _pLoop->queueLoop(this);///write complete
      }
   }
    if(n<static_cast<int>(msg.size())){
      _outBuf.append(msg.substr(n,msg.size()));
      if(_pChannel->isWriting()){//若是通道可以工作，注册一个IO读取Buffer数据的事件到epoll中
        _pChannel->enableWriting();//发送
      }
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
void TcpConnection::run(){
  _pUser->onWriteComplete(this);
}
