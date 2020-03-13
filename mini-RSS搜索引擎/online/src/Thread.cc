#include "../include/Thread.h"

#include<iostream>
using std::cout;
using std::endl;

namespace wd{

namespace current_thread{
__thread const char *threadName="wd thread";
//extern表示“外来的”意思
}
using ThreadCallback=std::function<void()>;

struct ThreadData//大概是用于存储每个线程特有的属性 
{
	ThreadData(const string name,ThreadCallback && cb)
	:_name(name)
	,_cb(cb)
	{}

	//子线程真正执行的函数
	//个人理解：给全局变量赋值一个局部值
	void runInThread(){
		cout<<"void runInThread()"<<endl; 
		current_thread::threadName=(_name==string()?"wd thread":_name.c_str());
		if(_cb){
			cout<<"_cb()"<<endl;
			_cb();
		}
	}
	string _name;
	ThreadCallback _cb;
};

Thread::~Thread()
{
	if (_isRunning) {
		pthread_detach(_pthid);
	}
	cout << "~Thread()" << endl;
}


void Thread::start() {
	cout<<"thread::start"<<endl;
	ThreadData *pdata=new ThreadData(_name,std::move(_cb));
	pthread_create(&_pthid,nullptr,threadFunc,pdata);
	//最后那个参数，为什么不能传this，也是个问题
	_isRunning = true;
}  
void Thread::join() {
	if (_isRunning) {
		pthread_join(_pthid, nullptr);
		_isRunning = false;
	}
}





void * Thread::threadFunc(void * arg)
{
	ThreadData * pData = static_cast<ThreadData *>(arg);
	if(pData)
		pData->runInThread();//执行任务

	delete pData;
	return nullptr;
}

}
