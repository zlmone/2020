#include"../include/TcpServer.h"


namespace wd{

    TcpServer::TcpServer(const string &ip,unsigned short port)
    :_acceptor(ip,port)
    ,_loop(_acceptor)
    {
        //_acceptor.ready();笔者版本，这个应该放在start里面
    }
    

    void TcpServer::setConnectionCallback(TcpConnectionCallback &&cb){
        _loop.setConnectionCallback(std::move(cb));
    }
    void TcpServer::setMessageCallback(TcpConnectionCallback &&cb){
        _loop.setMessageCallback(std::move(cb));
    }
    void TcpServer::setCloseCallback(TcpConnectionCallback &&cb){
        _loop.setCloseCallback(std::move(cb));
    }
    void TcpServer::start(){
        _acceptor.ready();//看老师的答案加上去的
        _loop.loop();
    }
    
}