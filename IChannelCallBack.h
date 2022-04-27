#ifndef ICHANNELCALLBACK 
#define ICHANNELCALLBACK
class IChannelCallBack{
  public:
        void virtual handleWrite()=0;
        void virtual handleRead()=0;
};
#endif
