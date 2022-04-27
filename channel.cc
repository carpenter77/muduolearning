#include<sys/epoll.h>
#include<iostream>
#include"channel.h"
#include "EventLoop.h"
Channel::Channel(EventLoop* loop,int sockfd)
  :_pLoop(loop),
    _sockfd(sockfd)
    ,_events(0),
     _index(-1)
    ,_revents(0)
    ,_callBack(nullptr){
}
void Channel::setCallBack(IChannelCallBack* callBack){
  _callBack=callBack;
}
void Channel::setRevents(int revents){
  _revents=revents;
}

void Channel::handleEvent(){
  if(_revents & EPOLLIN){
    _callBack->handleRead();
  }
  if(_revents & EPOLLOUT){
    _callBack->handleWrite();
  }
}
int Channel::getSockfd(){
  return _sockfd;
}
int Channel::getEvents()
{
  return _events;
}
void Channel::setIndex(int index){
  _index=index;
}
int Channel::getIndex(){
  return _index;
}
void Channel::enableReading()
{
  _events|= EPOLLIN;
  update();
}
void Channel::enableWriting()
{
  _events|= EPOLLOUT;
  update();
}
void Channel::disableWriting()
{
  _events|= ~EPOLLOUT;
  update();
}
bool Channel::isWriting(){
  return _events| ~EPOLLOUT;
}
void Channel::update(){
    _pLoop->update(this);
}
