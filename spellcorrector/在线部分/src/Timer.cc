#include"../include/Timer.h"

#include<unistd.h>
#include<errno.h>
#include<poll.h>
#include<sys/timerfd.h>

#include<iostream>
using std::cout;
using std::endl;


namespace wd{

Timer::Timer(int initialTime,int periodicTime,TimerCallback &&cb)
:_fd(createTimerfd())
,_initialTime(initialTime)
,_periodicTime(periodicTime)
,_cb(std::move(cb))
,_isStart(false)
{}

Timer::~Timer(){
    if(_isStart){
        stop();
    }
}

//本类的主函数
void Timer::start(){
    _isStart=true;
    setTimerfd(_initialTime,_periodicTime);

    struct  pollfd pfd;
    pfd.fd=_fd;
    pfd.events=POLLIN;//request event请求事件

    while (_isStart)
    {
        int nready=::poll(&pfd,1,5000);
        if(nready==-1&&errno ==EINTR){
            //EINTR表示中断错误 
            continue;
        }else if(nready ==-1){
            perror(">> poll");
            return;
        }else if(nready ==0){
            cout<<">>poll timeout!"<<endl;
        }else{
            if (pfd.events&POLLIN){
                handleRead();//处理事件，非要有read()吗？
                //如果不用read进行处理，那么每一次poll返回的时候，注册函数都会被激活。。。。水平触发
                if (_cb){
                    _cb();  //执行任务
                }          
            }
        }
    }
}

void Timer::stop()
{
    setTimerfd(0,0);
    _isStart=false;
}





//下面三个，为接口封装
int Timer::createTimerfd(){
    int fd=::timerfd_create(CLOCK_REALTIME,0);
    //timerfd_create会返回文件描述符
    //第一参设置相对时间CLOCK_REALTIME或者绝对时间CLOCK_MONOTONIC 
    //第二参用于设定是否阻塞，内核2.26版本后全部设施为0
    if(fd==-1){
        perror(">> timerfd_create");
    }
    return fd;
}


//两个参全为0时，将关闭定时器
void Timer::setTimerfd(int initialTime,int periodicTime){
    struct itimerspec value;
    value.it_value.tv_sec=initialTime;
    value.it_value.tv_nsec=0;
    value.it_interval.tv_sec=periodicTime;
    value.it_interval.tv_nsec=0;
//timerfd_settime(_fd,0,&value,nullptr)启动和停止计时器
//第二参用于选择相对定时器和绝对定时器
//第三参为结构体struct itimerspec {
//               struct timespec it_interval;  /* 周期性时间，每隔多久执行一次 */
//               struct timespec it_value;     /* 第一次开始执行的时间，之后周期执行 */
//            };
 //其中        struct timespec {
//                time_t tv_sec;                /* Seconds */
//                long   tv_nsec;               /* Nanoseconds */
//             };
//第三参为结构体struct itimerspec，用于获取定时器之前的设定时间，不需要就设置为空指针
    if(-1==timerfd_settime(_fd,0,&value,nullptr)){
        perror(">> timerfd_settime");
    }
}

void Timer::handleRead(){
    uint64_t howmany;
//timerFd文件描述符被监听到（即到了你设定的一个时间（循环）的时候）之后
//可以读取，read读到的是，超时的次数，即本处的howmany
    int ret=::read(_fd,&howmany,sizeof(howmany));
    if(ret!=sizeof(howmany)){
        perror(">> read");
    }
}


}