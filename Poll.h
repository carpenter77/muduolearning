#ifndef POLL
#define POLL
#include "Declare.h"
#include "define.h"
#include <vector>
#include "sys/epoll.h"
#include "channel.h"
using namespace std;
class Poll{
  public:
       Poll(); 
       ~Poll();
       void poll(vector<Channel*>* pChannels);
       void update(Channel* pChannel);
  private:
       int _epollfd;
       struct epoll_event _events[MAX_EVENTS];
};
#endif
