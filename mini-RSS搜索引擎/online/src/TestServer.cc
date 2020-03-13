#include"../include/TcpServer.h"
#include"../include/ThreadPool.h"
#include"../include/MyTask.h"
#include"../include/Configuration.h"

#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<iostream>
using std::cout;
using std::endl;
using std::string;


using namespace wd;
namespace wd{
//如果不定义这个全局变量，只在函数中定义ThreadPool，将会出现编译错误？？？？？？？
ThreadPool *gThreadpool=nullptr;
}
void onConnection(const wd::TcpConnectionPtr &conn){                                   //传参问题？？？？？？？？？？？？？？？？
    cout<<conn->toString()<<"has connected!"<<endl;
    conn->send("welcome to server.");
}





//仅仅是收发消息，需要我们采用线程池的工作线程
void onMessage(const wd::TcpConnectionPtr &conn){
    string msg = conn->receive();
    cout<<"on message"<<endl;
    cout<<">>receive msg from client:"<<msg<<endl;
    //业务逻辑处理交给线程池处理
    //decode
    //compute
    //encode

    int idx=1;
    wd::Configuration conf("../conf/conf.txt");
    MyTask task(msg,conn,3,conf);                           //这里之后两参要想办法变成变量传进去？？？？？？？？

    //拿到线程池之后，就将该任务交给线程池去执行
    
    //ThreadPool *pthreadpool;
    //pthreadpool->addTask(std::bind(&MyTask::process,task));
    gThreadpool->addTask(std::bind(&MyTask::process,task));
}

void onClose(const wd::TcpConnectionPtr &conn){
    cout<<"onclose......"<<endl;
    cout<<">>"<<conn->toString()<<"has closed!"<<endl;
}




int main(void){
    //cout<<"main函数启动"<<endl;
    wd::Configuration conf("../conf/conf.txt");
    cout<<"配置文件加载成功"<<endl<<endl<<endl;

    auto threadConf=conf.getThreadPoolConf();
    auto timerConf=conf.getTimerConf();
    auto netConf=conf.getNetConf();
    cout<<"配置文件加载成功"<<endl<<endl<<endl;

    ThreadPool  threadpool(threadConf.first,threadConf.second);
    threadpool.start();
    gThreadpool=&threadpool;
    cout<<"线程池启动成功"<<endl<<endl<<endl;

   
    wd::TcpServer server(netConf.first,netConf.second);
//这里传递函数的时候没有传参，大概是因为，留给最底层TcpConnection给他放参数，牛逼
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.setCloseCallback(onClose);
//注册函数的形参是从哪里传过去的？

    cout<<"配置文件传入下列内容："<<endl
        <<threadConf.first<<" "<<threadConf.second<<endl
        <<netConf.first<<" "<<netConf.second<<endl;
    server.start();

    return 0;
}
