#include"../include/TimerThread.h"


namespace wd{

TimerThread::TimerThread(int initialTime,int periodicTime,ThreadCallback && cb)
:_timer(initialTime,periodicTime,std::move(cb))
,_thread(std::bind(&Timer::start,&_timer))
{}
TimerThread::~TimerThread(){}

void TimerThread::start(){
    _thread.start();
}

void TimerThread::stop(){
    _timer.stop();
    _thread.join();
}

}