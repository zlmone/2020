/**
 * Project Untitled
 */


#ifndef _ACCEPTOR_H
#define _ACCEPTOR_H

#include"InetAddress.h"
#include"Socket.h"


namespace wd
{

class Acceptor {
public: 
    Acceptor(const string &ip,unsigned short port);
    
    void ready();
    int accept();
    int fd() const{return _listensock.fd();}

private: 
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    
    void bind();
    void listen();
    
    InetAddress _addr;
    Socket _listensock;
};

}

#endif //_ACCEPTOR_H