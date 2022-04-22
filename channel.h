#ifndef CHANNEL_H
#define CHANNEL_H

#include"Declare.h"
#include"IChannelCallBack.h"
class Channel{
  public:
          Channel(int epollfd,int sockfd);
          ~Channel();
          void setCallBack(IChannelCallBack* callBack);
          void handleEvent();
          void setRevents(int revent);
          int getSockfd();
          void enableReading();
  private:
          void update();
          int _epollfd;
          int _sockfd;//
          int _events;//要关注的事件
          int _revents;//已经发生的事件
          IChannelCallBack* _callBack;//
};
#endif
