#include<sys/epoll.h>
#include<iostream>
#include"channel.h"
#include "EventLoop.h"
Channel::Channel(EventLoop* loop,int sockfd)
  :_pLoop(loop),
    _sockfd(sockfd)
    ,_events(0)
    ,_revents(0)
    ,_callBack(NULL){
}
void Channel::setCallBack(IChannelCallBack* callBack){
  _callBack=callBack;
}
void Channel::setRevents(int revents){
  _revents=revents;
}

void Channel::handleEvent(){
  if(_revents & EPOLLIN){
    _callBack->OnIn(_sockfd);
  }
}
int Channel::getSockfd(){
  return _sockfd;
}
int Channel::getEvents()
{
  return _events;
}
void Channel::enableReading()
{
  _events|= EPOLLIN;
  update();
}
void Channel::update(){
    _pLoop->update(this);
}
