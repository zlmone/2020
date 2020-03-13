/**
 * Project Untitled
 */


#ifndef _SOCKETIO_H
#define _SOCKETIO_H


namespace wd
{

class SocketIO 
{
public: 
    explicit
    SocketIO(int fd);
    
    
    int readn(char* buff,int len);
    int readline(char *buff,int maxlen);
    

    int writen(const char* buff,int len);
    //int readint32(int&);
    
    //void write1Int32(int);

private:
    int _fd;
    int recvPeek(char *buff,int len);
};

}


#endif //_SOCKETIO_H