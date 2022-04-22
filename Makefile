all:
	g++ define.h IChannelCallBack.h Declare.h channel.h channel.cc TcpServer.h TcpServer.cc main.cc -o main
clean:
	rm *.o
