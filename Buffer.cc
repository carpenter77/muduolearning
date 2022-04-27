#include "Buffer.h"
Buffer::Buffer(){
}

Buffer::~Buffer(){}

const char* Buffer::peek(){
  return _buff.c_str();
}
int Buffer::readableBytes(){
  return static_cast<int>(_buff.size());
}
void Buffer::retrieve(int len){
    len=len>_buff.size()?_buff.size():len;
    _buff=_buff.substr(len,_buff.size());
}
void Buffer::append(const string& str){
  _buff.append(str);
}
string Buffer::retrieveAsString(int len){
  string result(peek(),len);
  retrieve(len);
  return result;
}
string Buffer::retrieveAllAsString(){
  return retrieveAsString(readableBytes());
}
