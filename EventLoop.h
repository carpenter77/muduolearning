#ifndef EVENTLOOP
#define EVENTLOOP
#include "Declare.h"
#include "Poll.h"
class EventLoop{
  public:
        EventLoop();
        ~EventLoop();
        void loop();
        void update(Channel* channel);
  private:
          bool quit;
          Poll* _poll;
};
#endif
