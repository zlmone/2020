#include"../include/TcpConnection.h"
#include"../include/InetAddress.h"
#include"../include/EventLoop.h"

#include<errno.h>
#include<stdlib.h>
#include<sys/socket.h>
 
#include<sstream>


namespace wd
{

TcpConnection::TcpConnection(int fd,EventLoop *loop) 
:_sock(fd)
,_sockIo(fd)
,_localAddr(getLocalAddr(fd))
,_peerAddr(getPeerAddr(fd))
,_isShutdownWrite(false)
,_loop(loop)
{}
TcpConnection::~TcpConnection() {
    if(!_isShutdownWrite){
        shutdown();
    }

}    



string TcpConnection::receive() {
    char buff[65536]={0};
    _sockIo.readline(buff,sizeof(buff));
    return string(buff);
}
void TcpConnection::send(const string &msg) {
    _sockIo.writen(msg.c_str(),msg.size());
}
void TcpConnection::sendInLoop(const string &msg){
    if(_loop){
        _loop->runInLoop(std::bind(&TcpConnection::send,this,msg));
    }
}




void TcpConnection::shutdown(){
    if(_isShutdownWrite){
        _isShutdownWrite=true;
        _sock.shutdownWrite();
    }
}
string TcpConnection::toString() const{
    std::ostringstream oss;
    oss<<_localAddr.ip()<<":"<<_localAddr.port()<<"-->"
        <<_peerAddr.ip()<<":"<<_peerAddr.port();
    return oss.str();
}



InetAddress TcpConnection::getLocalAddr(int fd){
    struct sockaddr_in addr;
    socklen_t len =sizeof(struct sockaddr);
    if(getsockname(_sock.fd(),(struct sockaddr*)&addr,&len)==-1){
        //getsockname(),may be a api,but I can't man it
        perror("getsockname");
    }
    return InetAddress(addr);   //the class has been defined before
}
InetAddress TcpConnection::getPeerAddr(int fd){
    struct sockaddr_in addr;
    socklen_t len =sizeof(struct sockaddr);
    if(getpeername(_sock.fd(),(struct sockaddr*)&addr,&len)==-1){
        perror("getpeername");
    }
    return InetAddress(addr);
}





void TcpConnection::setConnectionCallback(const TcpConnectionCallback & cb){
    _onConnection=std::move(cb);
}
void TcpConnection::setMessageCallback(const TcpConnectionCallback & cb){
    _onMessage=std::move(cb);
}
void TcpConnection::setCloseCallback(const TcpConnectionCallback & cb){
    _onClose=std::move(cb);
}

//？？？？？？？？？
//发现有回调函数，就进行执行
//用shared_from_this,是因为需要根据当前的TcpConnection对象完成的
void TcpConnection::handleConnectionCallback(){
    if(_onConnection){
        _onConnection(shared_from_this());
    }
}
void TcpConnection::handleMessageCallback(){
     if(_onMessage){
        _onMessage(shared_from_this());
    }   
}
void TcpConnection::handleCloseCallback(){
    if(_onClose){
        _onClose(shared_from_this());
    }
}





} // namespace wd
