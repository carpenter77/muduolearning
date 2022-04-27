#include "EventLoop.h"
#include <cstdio>
#include <iostream>
#include <cstdio>
#include <sys/eventfd.h>
#include <unistd.h>
#include "Poll.h"
EventLoop::EventLoop()
    :quit(false),
    _poll(new Poll())
{
}
EventLoop::~EventLoop(){}
void EventLoop::wakeup(){
  uint64_t one=1;
  ssize_t n=::write(_eventfd,&one,sizeof(one));
  if (n!= sizeof one){
    std::cerr<<"EvenetLoop::wakeup write failed"<<std::endl;
  }
}
void EventLoop::queueLoop(IRun* pRun){
  _pendingFunctors.push_back(pRun);
  wakeup();
}
void EventLoop::loop(){
  while(!quit){
    vector<Channel*> channels;
    printf("loop\n");
    _poll->poll(&channels);
    vector<Channel*>::iterator it;
    for(it=channels.begin();it!=channels.end();++it){
      (*it)->handleEvent();
    }
  }
}

void EventLoop::update(Channel* pChannel){
  _poll->update(pChannel);
}

void EventLoop::handleWrite(){
}

void EventLoop::handleRead(){
        uint64_t one=1;
        ssize_t n=::read(_eventfd,&one,sizeof(one));
        if (n!=sizeof(one)){
          std:cerr<<"EventLoop handleRead() read "<<n<<" bytes data"<<std::endl;
        }
}
void EventLoop::doPendingFunctors(){
  vector<IRun*> tempRuns;
  tempRuns.swap(_pendingFunctors);
  vector<IRun*>::iterator it;
  for(it=tempRuns.begin();it!=tempRuns.end();++it){
    (*it)->run();
  }
}
