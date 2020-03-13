/**
 * Project Untitled
 */


#ifndef _SOCKET_H
#define _SOCKET_H



namespace wd
{

class Socket {

public: 
    Socket();
    explicit    
    Socket(int fd);
    
    int fd() const;
    void shutdownWrite();
    //only shutdown the one side of the link,you can choose the side youself
    ~Socket();

private:
    int _fd;
};

}

#endif //_SOCKET_H