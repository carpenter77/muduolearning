#include <iostream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include<sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
struct pack{
    uint32_t length;
    char buf[1024];
};
#define ERR_EXIT(m) \
        do  \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while(0);

ssize_t readn(int fd,void *buf,size_t count){//返回成功读取的字节数
    size_t nleft=count;//剩余字节数
    ssize_t nread;
    char* bufp=(char*) buf;
    while(nleft>0){
        nread=read(fd,bufp,nleft);
        if(nread<0){
            if(errno==EINTR){ //如果errno==EINTR，表示系统当前中断
                continue;
            }
            return -1;
        }else if(nread==0){//若是收到close断开连接
            return count-nleft;
        }
        bufp+=nread; //改变读取的游标
        nleft-=nread;
    }
    return count;
}
ssize_t writen(int fd,const void* buf,size_t count){
    size_t nleft=count;
    ssize_t nwritten;
    char* bufp=(char*) buf;
    while(nleft>0){
        if((nwritten=write(fd,bufp,nleft))<0){
            if(errno==EINTR){
                continue;
            }
            return -1;
        }else if(nwritten==0){
            continue;
        }
        bufp+=nwritten;
        nleft-=nwritten;
    }
    return count;
}
void handle_sigchld(int sig)
{
    // wait(NULL);
    while (waitpid(-1, NULL, WNOHANG) > 0);
}
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
    while (1)
    {
        int ret = recv(sockfd, buf, len, MSG_PEEK); // 查看传入消息
        if (ret == -1 && errno == EINTR)
        {
            continue;
        }
        return ret;
    }
}

ssize_t readline(int sockfd, void *buf, size_t maxline)
{
    int ret;
    int nread;
    char *bufp = (char*)buf;    // 当前指针位置
    int nleft = maxline;
    while (1)
    {
        ret = recv_peek(sockfd, buf, nleft);
        if (ret < 0)
        {
            return ret;
        }
        else if (ret == 0)
        {
            return ret;
        }
        nread = ret;
        int i;
        for (i = 0; i < nread; i++)
        {
            if (bufp[i] == '\n')
            {
                ret = readn(sockfd, bufp, i+1);
                if (ret != i+1)
                {
                    exit(EXIT_FAILURE);
                }
                return ret;
            }
        }
        if (nread > nleft)
        {
            exit(EXIT_FAILURE);
        }
        nleft -= nread;
        ret = readn(sockfd, bufp, nread);
        if (ret != nread)
        {
            exit(EXIT_FAILURE);
        }
        bufp += nread;
    }
    return -1;
}
void ehco_cli(int sockfd)
{
    fd_set rset; //
    FD_ZERO(&rset);//清空 fd_set

    int nready;
    int maxfd;
    //标准输入和网络连接IO一块
    int fd_stdin = fileno(stdin); //生成一个标准输入描述符
    if (fd_stdin > sockfd)
    {
        maxfd = fd_stdin;
    } else {
        maxfd = sockfd;
    }

    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};

    while (1)  //检测是否产生可读事件
    {
        //放入到select的读集合中
        FD_SET(fd_stdin, &rset);
        FD_SET(sockfd, &rset);
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if (nready == -1)
        {
            ERR_EXIT("select");
        }
        if (nready == 0)
        {
            continue;
        }

        if (FD_ISSET(sockfd, &rset))    // sock数据 读取
        {
            int ret = readline(sockfd, recvbuf, sizeof recvbuf);    // 服务器读取
            if (ret == -1)
            {
                ERR_EXIT("readline");
            }
            if (ret == 0)
            {
                printf("server close\n");
                break;
            }

            fputs(recvbuf, stdout); // 服务器返回数据输出
            memset(recvbuf, 0, sizeof(recvbuf));
        }

        if (FD_ISSET(fd_stdin, &rset))
        {
            if (fgets(sendbuf, sizeof sendbuf, stdin) == NULL)   // 键盘输入获取
            {
                break;
            }
            writen(sockfd, sendbuf, strlen(sendbuf)); // 写入服务器
        }
    }
    close(sockfd);
};
int main(int argc, char** argv) {
    // signal(SIGCHLD, SIG_IGN);
    signal(SIGCHLD, handle_sigchld);
    // 1. 创建套接字
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        ERR_EXIT("socket");
    }

    // 2. 分配套接字地址
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof servaddr);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9000);
    // servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // inet_aton("127.0.0.1", &servaddr.sin_addr);

    // 3. 请求链接
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof servaddr) < 0) {
        ERR_EXIT("connect");
    }

    struct sockaddr_in localaddr;
    socklen_t addrlen = sizeof localaddr;
    if (getsockname(sockfd, (struct sockaddr*)&localaddr, &addrlen) < 0)
    {
        ERR_EXIT("getsockname");
    }
    printf("id = %s, ", inet_ntoa(localaddr.sin_addr));
    printf("port = %d\n", ntohs(localaddr.sin_port));

    // 4. 数据交换
    ehco_cli(sockfd);

    // 5. 断开连接
    close(sockfd);
    return 0;
}
