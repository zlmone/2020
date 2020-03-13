#pragma once

#include<functional>
#include<iostream>
using std::cout;
using std::endl;

namespace wd{

class Timer
{
public:
    using TimerCallback=std::function<void()>;
    
    Timer(int initialTime,int periodicTime,TimerCallback &&cb);
    ~Timer();

    void start();
    void stop();
    
private:
    int createTimerfd();
    void setTimerfd(int initialTime,int periodicTime);
    void handleRead();
    
private:
    int _fd;
    TimerCallback _cb;//要注册的功能
    int _initialTime;
    int _periodicTime;//给timerfd_settime传参用
    bool _isStart;
};

}


