/**
 * Project Untitled
 */
#ifndef _TCPCONNECTION_H
#define _TCPCONNECTION_H

#include"Socket.h"
#include"InetAddress.h"
#include"SocketIO.h"
#include"Noncopyable.h"
#include"EventLoop.h"


#include<string>
using std::string;
#include<memory>
#include<functional>
using std::function;


namespace wd
{

class TcpConnection;
using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using TcpConnectionCallback=function<void(const TcpConnectionPtr&)>;

class EventLoop;

class TcpConnection 
:Noncopyable
,public std::enable_shared_from_this<TcpConnection>
//这是让本类继承自上述enable_shared_from_this类，之后的shared_from_this()也是这个意思
{
public:
    TcpConnection(int fd,EventLoop *loop);
    ~TcpConnection();

    string receive();
    void send(const string &msg);
//版本4添加，sendInLoop()会调用EventLoop::runInLoop()
    void sendInLoop(const string &msg);
    void wakeup();

    
    string toString() const;
    void shutdown();



//newly added in v2,the function of callback
    void setConnectionCallback(const TcpConnectionCallback & cb);
    void setMessageCallback(const TcpConnectionCallback & cb);
    void setCloseCallback(const TcpConnectionCallback & cb);

    void handleConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();




private:
    InetAddress getLocalAddr(int fd);
    InetAddress getPeerAddr(int fd);



private: 
    Socket _sock;
    SocketIO _sockIo;
    InetAddress _localAddr;
    InetAddress _peerAddr;
    bool _isShutdownWrite;

//newly added in v2？？？？？？？？？
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;

    EventLoop *_loop;   //版本4添加
//TcpConnection需要知道EventLoop的存在，才能向EventLoop::_pendingFunctors中添加数据

};    
} // namespace wd

#endif 