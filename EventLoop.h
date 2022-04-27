#ifndef EVENTLOOP
#define EVENTLOOP
#include <vector>
#include "Declare.h"
#include "IRun.h"
#include "Poll.h"
class EventLoop: public IChannelCallBack{
  public:
        EventLoop();
        ~EventLoop();
        void loop();
        void update(Channel* channel);
        virtual void handleRead();
        virtual void handleWrite();
        void queueLoop(IRun*);
  private:
          void wakeup();
          int createEventfd();
          void doPendingFunctors();
          int _eventfd;
          vector<IRun*> _pendingFunctors;
          Channel* wakeupChannel;
          bool quit;
          Poll* _poll;
};
#endif
