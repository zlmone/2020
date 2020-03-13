/**
 * Project Untitled
 */
#pragma once
//#ifndef _THREADPOOL_H
//#define _THREADPOOL_H

#include"Thread.h"
#include"TaskQueue.h"

#include<vector>
#include<memory>
#include<functional>
using namespace std;





namespace wd{

using Task=std::function<void()>;
//实现的功能是线程池的管理器
class ThreadPool 
{    
public:
    //构造析构
    ThreadPool(size_t,size_t);
    ~ThreadPool();
    
//开启线程池的运行
    void start();
    void stop();    
//析构将调用stop()

//添加任务
    void addTask(Task && task);
    //函数类型为什么是抽象类指针，查55:23
private: 
    friend class WorkerThread;
    void threadFunc();
    Task getTask();

private:
    //线程容器
    size_t _threadNum;
    vector<unique_ptr<Thread>> _threads;
    
    //任务队列
    size_t _queSize;
    TaskQueue _taskque;
    
    //标志是否退出，如退出则标志位为true
    bool _isExit;

};


}


//版本1的bug：
//放在线程池里的任务,应该全部都执行完毕才退出
//      因为测试函数掉调用stop()时， 会将_isExit设置为true
//      在执行ThreadPool::threadFunc() 时,会因为isExit为true而无法进入while循环
//      之后那些还没被取干净的任务，因此将无法再被取走
//因此，在设置isExit为true之前，应该先判断当前的任务队列的任务数量是否减小为0，如没有，则应该等待
//因此，此时我们将stop()函数改为新款

//版本二的bug：程序无法退出
//      ThreadPool::threadFunc()可能process()执行过快，导致我们没有来得及将_isExit设置为true
//      ThreadPool::threadFunc()内进入循环体之后，调用getTask()方法，会TaskQue::pop（）任务队列
//      而TaskQue::pop（）会在任务队列TaskQue::empty()成立时，_notEmpty将持续循环调用wait(),导致一直等待，死锁
//  解决方案1：调用TaskQue::push(),其内有_notEmpty.notify()
//  解决方案2：_notEmpty.notifyAll()

