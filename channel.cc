#include<sys/epoll.h>
#include<iostream>
#include"channel.h"

Channel::Channel(int epollfd,int sockfd)
  :_epollfd(epollfd),
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
void Channel::enableReading()
{}
