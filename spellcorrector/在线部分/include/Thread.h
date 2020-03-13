#pragma once

#include"Noncopyable.h"
#include<pthread.h>

#include<functional>
#include<string>
using std::string;


namespace wd{

namespace current_thread{
extern __thread const char *threadName;
//extern表示“外来的”意思
}



class Thread 
:Noncopyable
{
public:
	using ThreadCallback=std::function<void()>;

	Thread(ThreadCallback &&cb,const string &name=string())
	: _pthid(0)
	, _isRunning(false)
	,_cb(std::move(cb))
	,_name(name)
	{}
	~Thread();
	
	void start();
	void join();

	static void * threadFunc(void *);
	string name() const{
		return _name;
	}
private: 
    pthread_t _pthid;	
    bool _isRunning;
	ThreadCallback _cb;
	string _name;
};

}
