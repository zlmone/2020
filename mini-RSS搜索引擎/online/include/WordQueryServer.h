/**
 * Project 基本网络编程工具
 */

/*
#ifndef _WORDQUERYSERVER_H
#define _WORDQUERYSERVER_H

class WordQueryServer {
public: 
   
   
void WordQueryServer(void const string &filename);
    

void onConnection(void const TcpConnectionPtr &conn);
    

void onMessage(void const TcpConnectionPtr &conn);
    

void onClose(void const TcpConnectionPtr &conn);
    

void doTaskThread(void const TcpConnectionPtr &conn, void const string &msg);
    
void start();
protected: 
    
void WordQueryServer();
private: 
    Configuration _conf;
    TcpServer _tcpServer;
    WordQuery _wordQuery;
    ThreadPool _threadPool;
};

#endif*/