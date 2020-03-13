#include"TcpServer.h"
#include"ThreadPool.h"
#include"WordQuery.h"
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<iostream>
using std::cout;
using std::endl;
using std::string;


using namespace wd;




//版本3添加
#if 1
//用Task这个名字好像是不行的，说是模棱两可
    class MyTask
    {
    private:
        string _msg;                //收到的消息，由下层传入
        wd::TcpConnectionPtr _conn;//链接指针
        int _rank;                  //要返回几个词
        Configuration _conf;

    public:
        MyTask(const string &msg,const wd::TcpConnectionPtr &conn,int formerKth,Configuration conf)
        :_msg(msg)
        ,_conn(conn)
        ,_rank(formerKth)
        ,_conf(conf){}
        
        void process(){
            //出错的情况
            cout<<"process 调试n+15次"<<endl;
            if(_msg.size()==0){
                string noword="what the fuck you talk about?";
                _conn->sendInLoop(noword);
                return;
            }
            cout<<">>"<<current_thread::threadName<<endl;//这个看上去也可以给线程做标示
            wd::WordQuery wq(_conf);
            string s=wq.doQuery(_msg);
            cout<<"发送数据"<<s;
            _conn->sendInLoop(s);//版本4
        }


    };
#endif
