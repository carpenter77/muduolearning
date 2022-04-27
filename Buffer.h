#ifndef BUFFER
#define BUFFER
#include<string>
using namespace std;
class Buffer{
  public:
          Buffer();
          ~Buffer();
          const char* peek();
          int readableBytes();
          void append(const string& str);
          void retrieve(int len);
          string retrieveAllAsString();
          string retrieveAsString(int len);
  private:
          string _buff;
};
#endif
