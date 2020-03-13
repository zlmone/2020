/**
 * Project Untitled
 */
#include "../include/ThreadPool.h"

#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;
/**
 * ThreadPool implementation
 */
//


namespace wd
{

ThreadPool::ThreadPool(size_t threadnum, size_t size) 
:_threadNum(threadnum)
,_queSize(size)
,_taskque(_queSize) //taskque的构造函数只需要传入size参数即可
,_isExit(false)
{
    //vector类型_threads可以调用其默认构造函数即可
    _threads.reserve(_threadNum);
    cout<<"ThreadPool::Threadpool(void size_t num, void size_t size)"
                                  <<endl;
}
ThreadPool::~ThreadPool()
{
	if(!_isExit) {
        cout<<"ThreadPool::~ThreadPool()"<<endl;
		stop();
	}
}






//注册_threadNum个数的线程，依次push到容器中，并将他们启动
void ThreadPool::start() {
    for(size_t idx=0;idx!=_threadNum;++idx){
        string threadName=std::to_string(idx);
        unique_ptr<Thread> thread(new Thread(
                        std::bind(&ThreadPool::threadFunc,this),threadName));
//这里this指针是ThreadPool类型，而WorkerThread正好它的构造函数的传入参数也是ThreadPool类型
//牛逼
        _threads.push_back(std::move(thread));
    }
    for(auto & thread:_threads){
        thread->start();
    }
} 



void ThreadPool::stop()  
{
    cout<<"void ThreadPool::stop() "<<endl;
    if(!_isExit){
        while(!_taskque.empty()){
            //只要队列不空，就一直放任务进去
            //cout<<"1211 is empty"<<_taskque.empty()<<endl;
            //::sleep(1);       //睡眠一秒，时间过长，会导致无法退出，版本二改为usleep（）
            //::usleep(100000);//版本二
            ::usleep(1);       //版本三，有bug 
        }

        cout<<"2322"<<endl;
        _isExit=true;

        _taskque.wakeup();          //在最终版本中添加，同时这一方法也要添加到TaskQueue类当中
    //TaskQue类此处添加了新的成员wakeup()
        cout << "3433" << endl;
   
   
        for(auto &thread:_threads){
            thread->join();
            //cout<<"join() success"<<endl;
        }
    }
}

//using Task=std::function<void()>;
void ThreadPool::addTask(Task && task) {
    cout<<"void ThreadPool::addTask(Task && task)"<<endl;
    _taskque.push(std::move(task));
}


Task ThreadPool::getTask() {
    cout<<"Task ThreadPool::getTask()"<<endl;
    return _taskque.pop();
}

/**
 * @return void
 */

//这个函数的具体代码还是需要你再研究一下,基本懂
void ThreadPool::threadFunc() {
    cout<<"void ThreadPool::threadFunc()"<<endl;
    while(!_isExit){
        Task task=getTask();//BO版本线程池，指针改为普通对象
        if(task){
            task();
        }
    }
}


}
//threadFunc()函数，将自身交给WorkerThread(),作为任务被调用执行
/**
 * @return Task
 */
