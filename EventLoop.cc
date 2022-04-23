#include "EventLoop.h"
#include <cstdio>
#include "Poll.h"
EventLoop::EventLoop()
    :quit(false),
    _poll(new Poll())
{
}
EventLoop::~EventLoop(){}

void EventLoop::loop(){
  while(!quit){
    vector<Channel*> channels;
    _poll->poll(&channels);
    printf("loop\n");
    vector<Channel*>::iterator it;
    for(it=channels.begin();it!=channels.end();++it){
      (*it)->handleEvent();
    }
  }
}
void EventLoop::update(Channel* pChannel){
  _poll->update(pChannel);
}
