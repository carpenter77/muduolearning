#include "EchoServer.h"
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
void EchoServer::onMessage(TcpConnection* pConn,const string& data){
  cout<<"onmessage"<<endl;
  pConn->send(data);
}
      
void EchoServer::onConnection(TcpConnection* pCon)
{
  cout << "onConnection" << endl;
}

