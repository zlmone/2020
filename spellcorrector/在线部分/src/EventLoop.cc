#include "../include/EventLoop.h"
#include"../include/Acceptor.h"
#include"../include/TcpConnection.h"

#include<assert.h>
#include<sys/eventfd.h>
#include<unistd.h>
#include<iostream>
using std::cout;
using std::endl;





namespace wd{

EventLoop::EventLoop(Acceptor &acceptor)
:_efd(createEpollFd())
,_eventfd(createEventFd())//V4
,_acceptor(acceptor)
,_eventList(1024)
,_isLooping(false)
{
    addEpollFdRead(_acceptor.fd());
    addEpollFdRead(_eventfd);//V4
}


void EventLoop::loop(){
//想要退出loop是不行的，你只能在另一个线程之中调用unloop来进行操作，？？？？？
    _isLooping=true;
    while (_isLooping)
    {
        waitEpollFd();
    }
}
void EventLoop::unLoop(){
    if(_isLooping){
        _isLooping=false;
    }
}












void EventLoop::waitEpollFd(){
    int nready; 
    //nready is the number of the fd,you detected
    do{
        nready=epoll_wait(_efd,&*_eventList.begin(),_eventList.size(),5000);
    //第二参，是对迭代器进行解引用，拿到元素之后再对其进行取地址，从而取得首地址 
    }while(nready==-1&&errno==EINTR);
    //轮循监听

    if(nready==-1){
        perror("epoll_wait");
        return;
    }else if(nready==0){
        cout<<">>epoll_wait time out!"<<endl;
    }else{
        if (nready==_eventList.size())//监听结果过多，要对容器进行扩容
        {   
            _eventList.resize(2*nready);
        }

        for (int idx = 0; idx != nready; ++idx)
        {
            int fd=_eventList[idx].data.fd;
            if(fd==_acceptor.fd())
            {//处理新链接
                if(_eventList[idx].events & EPOLLIN){
                    handleNewConnection();
                }
            }else if(fd==_eventfd){
                if(_eventList[idx].events&EPOLLIN){
                    handleRead();
                    cout<<"doPendingFunctors"<<endl;
                    doPendingFunctors();
                }
            }
            else
            {
                if (_eventList[idx].events & EPOLLIN)
                {
                    handleMessage(fd);
                }   
            }
        }
    } 
}

void EventLoop::addEpollFdRead(int fd){
    struct epoll_event evt;
    evt.data.fd =fd;
    evt.events=EPOLLIN;

    int ret=epoll_ctl(_efd,EPOLL_CTL_ADD,fd,&evt);
    if(-1==ret){
        perror("epoll_ctl");
    }
}

void EventLoop::handleNewConnection(){
    int peerfd=_acceptor.accept();
    addEpollFdRead(peerfd);

    TcpConnectionPtr conn(new TcpConnection(peerfd,this));
    //传递自身指针
    conn->setConnectionCallback(_onConnection);
    conn->setMessageCallback(_onMessage);
    conn->setCloseCallback(_onClose);

    _conns.insert(std::make_pair(peerfd,conn));
    conn->handleConnectionCallback();
}

void EventLoop::handleMessage(int fd){
    bool isClosed = isConnectionClosed(fd);
	auto iter = _conns.find(fd);
	assert(iter != _conns.end());//运行时断言

	if(!isClosed) {
		iter->second->handleMessageCallback();
	} else {
		delEpollFdRead(fd);
		iter->second->handleCloseCallback();
                //之后实际会执行的是onclose()函数，就是在主函数中注册的那一个 
		_conns.erase(iter);//擦除map中的相应元素
	}
}


bool EventLoop::isConnectionClosed(int fd){
    //如果recv的返回值是0，则表示链接已经断开
    int ret;
    do{
        char buff[1024];
        ret=recv(fd,buff,sizeof(buff),MSG_PEEK);
    }while (ret==-1&&errno==EINTR);
    
    return (ret==0);
}


int EventLoop::createEpollFd(){
    int ret=::epoll_create1(0);
    if(ret==-1){
        perror("epoll_create1");
    }
    return ret;
}





void EventLoop::delEpollFdRead(int fd){
    struct epoll_event evt;
    evt.data.fd =fd;

    int ret=epoll_ctl(_efd,EPOLL_CTL_DEL,fd,&evt);
    if(-1==ret){
        perror("epoll_ctl");
    }
}


int EventLoop::createEventFd(){
    int fd=::eventfd(0,0);
    if(fd==-1){
        perror(">> eventfd");
    }
    return fd;
}

//和Eventfd类一样的
void EventLoop::handleRead(){
    uint64_t howmany;
    int ret=::read(_eventfd,&howmany,sizeof(howmany));
    if(ret!=sizeof(howmany)){
        perror(">>read");
    }
}

void EventLoop::wakeup(){
    uint64_t one=1;
    int ret=::write(_eventfd,&one,sizeof(one));
    if(ret!=sizeof(one)){
        perror(">>write");
    }
}


void EventLoop::runInLoop(Functor && cb){
    {//加这个语句块是为了减小加锁的范围
        MutexLockGuard autolock(_mutex);
        _pendingFunctors.push_back(std::move(cb));
    }
    wakeup();
}


void EventLoop::doPendingFunctors (){
    vector<Functor> tmp;
    //_pendingFunctors,存储压力过大，因此用tmp将它转储
    //swap会将当前_pendingFunctors中的元素，全部转到tmp中
    //如此，可以保证_pendingFunctors加锁时间不会过长
    { 
        //加锁防止同时有子线程再向容器中放任务 
        MutexLockGuard autolock(_mutex);
        _pendingFunctors.swap(tmp);
    }

    for(auto &functor:tmp){
        functor();
    }
}

}
        