#pragma once
#include"Timer.h"
#include"Thread.h"

namespace wd{

class TimerThread
{
public:
    using ThreadCallback = std::function<void()>;

    TimerThread(int,int,ThreadCallback && cb);
    ~TimerThread();

    void start();
    void stop();

private:
    Timer _timer;
    Thread _thread;
};

}
