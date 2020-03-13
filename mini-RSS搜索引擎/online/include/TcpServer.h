#ifndef _WD_TCPSERVER_H_
#define _WD_TCPSERVER_H_


#include"Acceptor.h"
#include"EventLoop.h"
#include"TcpConnection.h"

#include<functional>


namespace wd{

class TcpServer
{
using TcpConnectionCallback=std::function<void(const TcpConnectionPtr&)>;
using TcpConnectionPtr=std::shared_ptr<TcpConnection>;

private:
    Acceptor _acceptor;
    EventLoop _loop;
public:
    TcpServer(const string &ip,unsigned short port);
    //~TcpServer();

    void setConnectionCallback(TcpConnectionCallback &&cb);
    void setMessageCallback(TcpConnectionCallback &&cb);
    void setCloseCallback(TcpConnectionCallback &&cb);

    void start();
};

}


#endif

