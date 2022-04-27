#include "EchoServer.h"

#define MESSAGE_LENGTH 8

EchoServer::EchoServer(EventLoop* loop)
                  : _loop(loop),
                  _pServer(loop)
{
    _pServer.setCallback(this);//内存泄漏的风险
}  
EchoServer::~EchoServer(){}
void EchoServer::start(){
  _pServer.start();
}
void EchoServer::onMessage(TcpConnection* pConn,Buffer* buffer){
  cout<<"onmessage"<<endl;
  string message= buffer->retrieveAllAsString();
  pConn->send(message);
}
      
void EchoServer::onConnection(TcpConnection* pCon)
{
  cout << "onConnection" << endl;
}

void EchoServer::onWriteComplete(TcpConnection* pCon)
{
  cout << "onWriteComplate" << endl;
}
