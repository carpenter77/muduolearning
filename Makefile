all:
	g++ define.h IChannelCallBack.h Declare.h IRun.h IMuduoUser.h Buffer.h Buffer.cc EchoServer.h EchoServer.cc channel.h channel.cc EventLoop.h EventLoop.cc Poll.h Poll.cc IAcceptorCallBack.h IChannelCallBack.h TcpConnection.h TcpConnection.cc Acceptor.h  Acceptor.cc  TcpServer.h TcpServer.cc main.cc -o main
clean:
	rm *.o
