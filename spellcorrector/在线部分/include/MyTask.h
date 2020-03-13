#include"TcpServer.h"
#include"ThreadPool.h"
#include"MyDict.h"
#include"KthLargestString.h"
#include"Cache.h"
#include"CacheManger.h"
#include"jsonPackage.h"

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

        //wd::Cache _cache;           //线程缓存v3舍弃
        //const string _pathname;      //缓存初始化文件路径            以后可能会删除
        CacheManger &_cacheMgr;

    public:
        MyTask(const string &msg,const wd::TcpConnectionPtr &conn,int formerKth,
                                        CacheManger &cacheMgr)
        :_msg(msg)
        ,_conn(conn)
        ,_rank(formerKth)
        ,_cacheMgr(cacheMgr)
        //,_cache(wd::CacheManger::getCache(idx)) //v3:应该修改成这样的形式，那么manager初始化要放在外面
        //,_pathname(pathName)
        {
            //_cache.readFromFile(pathName);
            //Cache功能仅仅需要这一个，即可，后面的周期回写，应该是全自动才对
            //cout<<"MyTask:cacheMgr.size() is "<<sizeof(_cacheMgr)<<endl;
            //cout<<"MyTask:cacheMgr._path is "<<_cacheMgr._path<<endl;
            cout<<"MyTask:cacheMgr._cachelist.size() is "<<_cacheMgr._cacheList.size()<<endl;
            cout<<"MyTask:cacheMgr._Thread2Cache.size() is "<<_cacheMgr._Thread2Cache.size()<<endl<<endl;
            _cacheMgr.print();
        }
        
        void process(){
            //出错的情况
            cout<<"process 调试n+15次"<<endl;
            if(_msg.size()==0){
                string noword="what the fuck you talk about?";
                _conn->sendInLoop(noword);
                return;
            }
            cout<<":task process()"<<endl;
            //cout<<"MyTask:cacheMgr._path is "<<_cacheMgr._path<<endl;//不注释掉，会疯狂打印
            cout<<"MyTask:cacheMgr._cachelist.size() is "<<_cacheMgr._cacheList.size()<<endl;
            cout<<"MyTask:cacheMgr.Thread2Cache.size() is "<<_cacheMgr._Thread2Cache.size()<<endl<<endl;
            
            cout<<">>"<<current_thread::threadName<<endl;//这个看上去也可以给线程做标示
            string response=answer(_msg,_rank/*,_pathname*/);

            _conn->sendInLoop(response);//版本4
        }



//问题应该出现在answer后面
        string answer(const string &message,int num/*,const char* filename*/){//怎么传后两个参进去也是个问题？？？？？？
           //定义一个单例字典类
            wd::MyDict *p1=wd::MyDict::createInstance();
            p1->printAll();
            //p1->init(filename);  

            Cache curCache=_cacheMgr.getCache();
            string res=curCache.findTheWord(message);
            if(res!=""){
                cout<<"Task::answer:cache got it!"<<endl;
                return  res;
            }else{


                
                cout<<"Task::answer:cache no found"<<endl;
                //以此定义一个查询类;
                wd::KthLargestString findWord(num,p1->getDict(),p1->getIndexTable());       
            //进行查询即可
                res="";//新加的
                vector<string> vs=findWord.retHeap(message);
                wd::jsonPackage package;
                res=package.packageJson(vs);  
                /*for(auto s:vs){
                    res=res+" "+s;
                }*/
                //(_cacheMgr.getCache()).addElement(message,res);    
                cout<<"更新后的缓存内容"<<endl;
                (_cacheMgr.getCache()).printTable();
                return res;
            }  
            
        }

    };
#endif
