#include "EventLoop.h"
#include "errno.h"
#include "unistd.h"
#include "Poll.h"
#include "define.h"
#include "sys/epoll.h"
#include "sys/types.h"
#include "stdlib.h"
#include "stdio.h"
#include "errno.h"
#include<vector>

const int kNew=-1;
const int kAdd=1;

Poll::Poll(){
	 _epollfd=epoll_create(1);
    if(_epollfd<0){
      perror("epllo_create");
      exit(1);
    }
}
Poll::~Poll(){}

void Poll::poll(vector<Channel*>* pChannels){
	  int fd=epoll_wait(_epollfd,_events,MAX_EVENTS,-1);
		if(fd==-1){
        perror("epoll_wait");
        exit(1);
      }
    for(int i=0;i<fd;i++){
          Channel* pChannel=static_cast<Channel*>(_events[i].data.ptr);
          pChannel->setRevents(_events[i].events);
          pChannels->push_back(pChannel);
   }
	
}

void Poll::update(Channel *pChannel){
  int index=pChannel->getIndex();
	struct epoll_event ev;
  ev.data.ptr=pChannel;
  ev.events=pChannel->getEvents();
  int fd=pChannel->getSockfd();

  if(index==kNew){
    pChannel->setIndex(1);
    ::epoll_ctl(_epollfd,EPOLL_CTL_ADD,fd,&ev);
  }else{
     ::epoll_ctl(_epollfd,EPOLL_CTL_MOD,fd,&ev);
  }
}
