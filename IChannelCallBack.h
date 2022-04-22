#ifndef ICHANNELCALLBACK 
#define ICHANNELCALLBACK
class IChannelCallBack{
  public:
        void virtual OnIn(int sockfd){};
};
#endif
