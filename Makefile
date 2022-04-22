all:
	g++ define.h IChannelCallBack.h Declare.h channel.h channel.cc IAcceptorCallBack.h IChannelCallBack.h TcpConnection.h TcpConnection.cc Acceptor.h  Acceptor.cc  TcpServer.h TcpServer.cc main.cc -o main
clean:
	rm *.o
