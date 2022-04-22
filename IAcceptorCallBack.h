#ifndef IACCEPTORCALLBACK
#define IACCEPTORCALLBACK
class IAcceptorCallBack{
  public:
          virtual void newConnection(int sockfd){};
};
#endif

