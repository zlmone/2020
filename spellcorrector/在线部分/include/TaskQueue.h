/**
 * Project Untitled
 */


//#ifndef _TASKQUEUE_H
//#define _TASKQUEUE_H

#pragma once

//#include"queue.h"暫時不用，使用標準模板庫
#include"Condition.h"
#include"MutexLock.h"

#include<queue>
#include<functional>
using std::queue;


//bo改造完成
namespace wd{

using ElemType = std::function<void()>;

class TaskQueue {
private:
    //Queue<int> _que;
    queue<ElemType> _que;
    size_t _queSize;
    MutexLock _mutex;
    Condition _notFull; //功能相当于伪代码中的empty
    Condition _notEmpty;//功能相当于伪代码中的full
    bool _flag;

public:
    TaskQueue(size_t size);

	ElemType pop();    
	void push(ElemType elem);    
	bool full() const;
	bool empty() const;

    void wakeup();
};


}
