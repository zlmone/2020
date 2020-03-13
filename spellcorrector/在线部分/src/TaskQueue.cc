#include "../include/TaskQueue.h"
#include<iostream>
using namespace std;
/**
 * TaskQueue implementation
 */

//bo改造完成


namespace wd{
	
TaskQueue::TaskQueue(size_t size)
:_queSize(size)
,_mutex()
,_notFull(_mutex)
,_notEmpty(_mutex)
,_flag(true)
{
    cout<<"TaskQueue::TaskQueue(size_t size)"<<endl;
}


bool TaskQueue::full() const{ 
    return _que.size()==_queSize;
}
bool TaskQueue::empty() const{
    return _que.size()==0;
}





ElemType TaskQueue::pop() {
    MutexLockGuard autoLock(_mutex);
    while(_flag&&empty()){     // //使用while 是为了防止虚假(异常)唤醒,疑点？？？？
        _notEmpty.wait();
    }

    if(_flag){
        ElemType elem=_que.front();
        _que.pop();
        _notFull.notify();
        return elem;
    }else{
        return nullptr;
    }
    
/*笔者写法 if(_que.empty()){
        cout<<"the queue is empty!"<<endl;
    }else{
        //出棧
        //p(full)
        _notEmpty.wait();
        return _que.front();    //这里错误，要用变量暂存
        _notFull.notify();
        //v(empty)
        _que.pop();
        
    }
    return 0;*/
}


//using ElemType = std::function<void()>;
void TaskQueue::push(ElemType elem) {
    //p(empty)
    cout<<"void TaskQueue::push(ElemType elem)"<<endl;
    MutexLockGuard autolock(_mutex);
    while (full())
    {
        _notFull.wait();
    }    
    //入隊
    _que.push(elem);
    _notEmpty.notify();
    //v(full)
    return ;
}


void TaskQueue::wakeup(){
    _flag=false;
    _notEmpty.notifyAll();
}

}