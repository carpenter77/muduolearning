#include "TcpServer.h"
#include<vector>

TcpServer::TcpServer(EventLoop* ploop)
        : _pAcceptor(nullptr),
        _pLoop(ploop),
        _user(nullptr){
}
TcpServer::~TcpServer()
{
}
void TcpServer::newConnection(int sockfd){
  TcpConnection* p=new TcpConnection(_pLoop,sockfd);
  _connections[sockfd]=p;
  p->setUser(_user);
  p->connectEstablished();
}
void TcpServer::start(){
   _pAcceptor=new Acceptor(_pLoop);
  _pAcceptor->setCallBack(this);
  _pAcceptor->start();
}
void TcpServer::setCallback(IMuduoUser* user){
  _user=user;
}
