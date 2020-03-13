
#ifndef _CACHEMANGER_H
#define _CACHEMANGER_H

#include<vector>
#include<map>
#include<string>
#include<unordered_map>
#include"Cache.h"
#include<pthread.h>
#include<iostream>
using namespace std;

namespace wd{
    
//看上去是用一个线程来维护多个cache
//这个线程估计是专门用于写的
//所以应该是全程只有一个磁盘缓存文件
class CacheManger {
public: 
CacheManger(const string path):_path(path){
    //cout<<"调用错误构造函数 ";
    cout<<"_path="<<_path<<endl;
}
/*
CacheManger(CacheManger *cacheMgr)
:_cacheList(cacheMgr->_cacheList)
,_Thread2Cache(cacheMgr->_Thread2Cache)
,_path(cacheMgr->_path)
{
    cout<<"调用赋值构造函数"<<endl;
    cout<<"CacheManger(CacheManger *cacheMgr):映射文件大小"<<_Thread2Cache.size()<<endl;
}*/
//这个是用来初始化一个，还是初始化所有的？
void initCache();
Cache& getCache();    

//文档让做静态函数，但是示例代码没有做，而且用静态函数我没办法bind
void periodicUpdate();//传参是注册函数需要
void print();

void updateCache(size_t i,Cache c);

private:
unordered_map<string,string> eliminateDuplicate(unordered_map<string,string> indexLine);
unordered_map<string,string> loadFile();
//定时更新所有缓存

public: 
    vector<Cache> _cacheList;
    map<pthread_t,size_t> _Thread2Cache;
    string _path;
    //可能还要加减锁
};



}

#endif //_CACHEMANGER_H
