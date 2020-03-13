/**
 * Project Untitled
 */
#include "../include/Acceptor.h"
#include <sys/types.h>
#include <sys/socket.h>




namespace wd
{
Acceptor::Acceptor(const string & ip,unsigned short port) 
:_addr(ip,port)
,_listensock()
{
}



void Acceptor::ready() {
    setReuseAddr(true);
	setReusePort(true);
    bind();
    listen();
}



/**
 * @param bool on
 * @return void
 */
void Acceptor::setReuseAddr(bool on) {
    int one=on;
    //setsockopt用于，对套接口进行各种各样的设置
    //      要在bind之前进行设置
    if(setsockopt(_listensock.fd()
                    ,SOL_SOCKET
                    ,SO_REUSEADDR
                    ,&one
                    ,sizeof(one))<0){
                        perror("setsockopt");
                    }
}

/**
 * @param bool on
 * @return void
 */
void Acceptor::setReusePort(bool on) {
	int one = on;
	if(setsockopt(_listensock.fd(), 
		SOL_SOCKET, 
		SO_REUSEPORT, 
		&one, 
		sizeof(one)) < 0) {
		    perror("setsockopt");	
	    }
}







void Acceptor::bind() 
{
    //use :: incase it will apply the bind function,
    //                  which belongs to its own class
	int ret = ::bind(_listensock.fd(), 
		            (struct sockaddr *)_addr.getInetAddressPtr(),
		            sizeof(struct sockaddr));
                //二参，为什么是指针，为什么不行???????????????????
                //二参的函数我man不出来
    if(-1==ret){
        perror("bind");
    }
}

/**
 * @return void
 */
void Acceptor::listen() {
    int ret=::listen(_listensock.fd(),10);//第二参为同时处理的最大连接数，一般为10或者15
    if(-1==ret){
        perror("listen");
    }
}


int Acceptor::accept() {
    int peerfd=::accept(_listensock.fd(),NULL,NULL);
                    //          第二参为传出参数，传出sockaddr型信息，第三参为第二参的大小???????????为什么是ＮＵＬＬ
    if(-1==peerfd){
        perror("accept");
    }
    return peerfd;
}


}



