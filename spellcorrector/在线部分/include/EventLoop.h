#ifndef _WD_EVENTLOOP_H_
#define _WD_EVENTLOOP_H_

#include<sys/epoll.h>
#include"MutexLock.h"

#include<vector>
using std::vector;
#include<map>
using std::map;
#include<memory>
using std::shared_ptr;
#include<functional>



namespace wd{
     class Acceptor;
     class TcpConnection;

     class EventLoop
     {
     public:
     //类型别名
         using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
         using TcpConnectionCallback=std::function<void(const TcpConnectionPtr&)>;
         using Functor=std::function<void()>;
         
         EventLoop(Acceptor &acceptor);
     	   void loop();			//调用waitEpollFd()
         void unLoop();
         void runInLoop(Functor &&cb);//V4


         void setConnectionCallback(TcpConnectionCallback &&cb){
            _onConnection=std::move(cb);}
         void setMessageCallback(TcpConnectionCallback &&cb){
            _onMessage=std::move(cb);}
         void setCloseCallback(TcpConnectionCallback &&cb){
            _onClose=std::move(cb);}


     private:
         void waitEpollFd();		      //将调用后面两个监听情况        
         void handleNewConnection();	//epoll监听情况1：新客户端到来
         void handleMessage(int fd);	//epoll监听情况2：客户端发送消息过来
                                       //会调用delEpollFdRead(int fd)
         int createEpollFd();		      //就建立epoll呗
         void addEpollFdRead(int fd);  
         void delEpollFdRead(int fd);  
         bool isConnectionClosed(int fd);

         //版本4
         int createEventFd();//给——eventfd配套使用
         void handleRead();//版本4：内部拥有读功能
         void wakeup();    //版本4：内部拥有写功能
         //将_pendingFunctors中所有保存的注册函数全部执行一遍
         void doPendingFunctors();



     private:
         int _efd;
         Acceptor &_acceptor;
         vector<struct epoll_event> _eventList;//大概就是存储监听的事件
         map<int,TcpConnectionPtr> _conns;     //管理你建立的连接
         bool _isLooping;                      //用于防止死循环
 

   //handleNewConnection是没有参数的，因此参数会通过成员传入
        TcpConnectionCallback _onConnection;
        TcpConnectionCallback _onMessage;
        TcpConnectionCallback _onClose;
        

        int _eventfd;  //版本4添加同时配套两个函数handleRead()和wakeup()
         //主文件注册的process方法，要想办法拿到eventfd，调用wakeup，通知epollwait
        vector<Functor> _pendingFunctors;
               //TcpConnection（自线程）通过回调函数将任务交给EventLoop
        MutexLock _mutex;
               //_pendingFunctors为临界资源，要为它添加互斥锁
         
         
        
     }; 
    }

#endif
