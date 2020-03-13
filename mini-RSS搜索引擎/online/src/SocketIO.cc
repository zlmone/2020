/**
 * Project Untitled
 */


#include "../include/SocketIO.h"
#include <stdio.h>
#include<iostream>
using namespace std;
#include <unistd.h>
            //读写操作的头文件
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>



namespace wd
{


SocketIO:: SocketIO(int fd) 
:_fd(fd)
{
}



//这个函数的功能也要看
int SocketIO::recvPeek(char *buff,int len){
    int ret;
    do{
        ret=::recv(_fd,buff,len,MSG_PEEK);
    }while(ret==-1&&errno==EINTR);//跟下面那个问题是一样的
    return ret;
}


//这个函数，有空给我重新看,问题主要出在标注的那里
//返回的是已经读取的长度
int SocketIO::readn(char* buff,int len) {
    int left=len;
    char *p=buff;
    while(left>0){
        int ret=::read(_fd,p,left);
        if(ret=-1&&errno==EINTR){       //??????????????????????????????
            continue;        
        }
        else if(ret==-1)
        {
            perror("read");
            return len-left;
        }
        else if(ret==0){
            //此时说明读取结束
            return len-left;
        }else{
            left -=ret;
            p+=ret;
        }
    }
    return len-left;
    //at last,"left" will become zero,so the return value will be"len"
}



//这个没搞懂？？？？？？？？？？？？？？？？？？？？？？？？？？？？？？
int SocketIO::readline(char *buff,int maxlen) {
    int left=maxlen-1;
//because the last elem will be set as \0 
    char *p=buff;
    int ret;
    int total=0;
//the lenth of the data you finally get

    while(left>0){
        ret=recvPeek(p,left);
        //step jia
        for(int idx=0;idx!=ret;++idx){
            if(p[idx]=='\n'){
                int sz=idx+1;
                readn(p,sz);  
    //step yi:读这么一行,cause you have calculate the lenth of the line
                                //为什么没有perror？？？？？？？？？
                total+=sz;
                p+=sz;          //已经读入的大小
                *p='\0';        //？？？？？？？？？？？？？？？？？
                return total;
            }
        }
        //when the if condition is not established
        readn(p,ret);       //最后剩下的零头
        left-=ret;
        p+=ret;
        total+=ret;
    }
    *p='\0';
    return total;
}


int SocketIO::writen(const char* buff,int len) {
    cout<<"SocketIO::writen,strlen="<<len;
    int left=len;
    const char *p=buff;
    while(left>0){
        int ret=::write(_fd,p,left);
        if(ret==-1&&errno==EINTR){
            continue;
        }else if(ret == -1) {
			perror("write");
			return len - left;
		} else {
			left -= ret;
			p += ret;
		}
	}
	return len - left;
}




/*
int SocketIO::readint32(int&) {
    return 0;
}
void SocketIO::write1Int32(int) {
    return;
}
*/    


}

