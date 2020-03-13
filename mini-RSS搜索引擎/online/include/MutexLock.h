#pragma once

#include<pthread.h>
#include"Noncopyable.h"

namespace wd{

class MutexLock
:Noncopyable
{
public:
	MutexLock();
	~MutexLock();

	void lock();
	void unlock();

	pthread_mutex_t * getMutexLockPtr(){	return &_mutex;}	

private:
	pthread_mutex_t _mutex;
};

class MutexLockGuard
{
public:
	MutexLockGuard(MutexLock & mutex)
	: _mutex(mutex)
	{
		_mutex.lock();
	}

	~MutexLockGuard()
	{
		_mutex.unlock();
	}

private:
	MutexLock & _mutex;
};


}
	
