/**
 * Project Untitled
 */


#ifndef _INETADDRESS_H
#define _INETADDRESS_H

#include<string>
#include<arpa/inet.h>
        //大概是用来地址端口转换
using std::string;


namespace wd{
//主要实现地址端口的，格式转换，和获取
class InetAddress {
public: 
    
/**
 * @param const string &ip
 * @param unsigned short port
 */
        InetAddress(unsigned short port);
        InetAddress(const string &ip,unsigned short port);
        InetAddress(const struct sockaddr_in &addr);
    
    //the function below are set to return data you want
        string ip() const;
        unsigned short port() const;
        struct sockaddr_in *getInetAddressPtr(){ return &_addr;}
        //it was defined for the later class acceptor

private: 
        struct sockaddr_in _addr;
};

}


#endif //_INETADDRESS_H