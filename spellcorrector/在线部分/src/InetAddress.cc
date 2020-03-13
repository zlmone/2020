/**
 * Project Untitled
 */


#include "../include/InetAddress.h"
#include<string.h>
/**
 * InetAddress implementation
 */


/**
 * @param const string &ip
 * @param unsigned short port
 */
namespace wd
{

//this function default
InetAddress::InetAddress(unsigned short port){
    ::memset(&_addr,0,sizeof(struct sockaddr_in));
    _addr.sin_family=AF_INET;
    _addr.sin_port=htons(port);
    _addr.sin_addr.s_addr=INADDR_ANY;
    //bind to local address
}

InetAddress::InetAddress(const string &ip,unsigned short port) {
    ::memset(&_addr,0,sizeof(struct sockaddr_in));
    _addr.sin_family = AF_INET;//***************************这里是在设置为IPv4协议

    _addr.sin_port = htons(port);//将主机字节序转化为网络字节序
    _addr.sin_addr.s_addr=inet_addr(ip.c_str());
}

/**
 * @param const struct sockaddr _in & addr
 */
InetAddress::InetAddress(const struct sockaddr_in &addr) 
:_addr(addr)
{
}

/**
 * @return string
 */
string InetAddress::ip() const{
    return string(::inet_ntoa(_addr.sin_addr));
    //32位网络字节序二进制数值，转换为点分十进制
}

/**
 * @return unsigned short
 */
unsigned short InetAddress::port() const{
    return ntohs(_addr.sin_port);
    //字节序转换

}


}