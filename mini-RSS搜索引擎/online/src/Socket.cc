/**
 * Project Untitled
 */


#include "../include/Socket.h"
#include <stdio.h>

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>     //socket()要用到上面两个头文件
/**
 * Socket implementation
 */

namespace wd
{

    Socket::Socket() {
        _fd=socket(AF_INET,SOCK_STREAM,0);
        if(_fd==-1){
            perror("socket");
        }
    }
    Socket::Socket(int fd) 
    :_fd(fd)
    {
    }


    void Socket::shutdownWrite() {
        ::shutdown(_fd,SHUT_WR);        //shutdown可以用于实现半关闭
                                        //第二参数设定shutdown的方式
    }

/**
 * @return int
 */
    int Socket::fd() const{
        return _fd;
    }

    Socket::~Socket(){
        ::close(_fd);
    }

}
