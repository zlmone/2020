
#include"../include/CacheManger.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include<map>
#include<utility>
#include<pthread.h>
using namespace std;

#define LRUSIZE 10;







namespace wd{


int size=LRUSIZE;

//写缓存去重，是可能还需要优化的功能点
void CacheManger::initCache() {
    Cache cache(size);
    cache.readFromFile(_path);
    _cacheList.push_back(cache);
    cout<<"CacheManger::initCache():_Thread2Cache.size() is "<<_Thread2Cache.size()<<endl;
    cout<<"缓存队列大小"<<_cacheList.size()<<endl<<endl;
    return;
}


//v4版本重写
Cache& CacheManger::getCache() {
    auto it=_Thread2Cache.find(pthread_self());
    cout<<"Cache& CacheManger::getCache():pthread_self()="<<pthread_self()<<endl;
    if(it==_Thread2Cache.cend()){
        cout<<"缓存队列大小"<<_cacheList.size()<<endl;
        pair<pthread_t,size_t> p=make_pair(pthread_self(),_cacheList.size());
        cout<<"线程映射文件大小"<<_Thread2Cache.size()<<endl;
        /*if(_Thread2Cache.size()==0){
            _Thread2Cache.insert(p);
        }*/
        _Thread2Cache.insert(p);
        cout<<"插入成功"<<endl;
        Cache newCache(size);
        cout<<"将从临时cache导入文件"<<endl;
        newCache.readFromFile(_path);
        cout<<"将插入缓存队列中"<<endl;
        _cacheList.push_back(newCache);
        cout<<"对迭代器重新赋值"<<endl;
        it=_Thread2Cache.find(pthread_self());
    }
    cout<<"judge it"<<endl;
    cout<<it->first<<" "<<it->second<<endl;
    return _cacheList[it->second];
}


//更新缓存，暂时会叠加写上去，因此可能导致词语条目的，查询结果重复的问题
//可能需要添加一个去重的功能，去重功能暂时没有放进去，写在后面两个函数里面
void CacheManger::periodicUpdate() {
    cout<<"void CacheManger::periodicUpdate(const int i)"<<endl<<endl;
    auto it=_cacheList.begin();
    for(;it!=_cacheList.end();++it){
        //it->update();
        //理论上应该是，写回到自己的文件中
        //cout<<"写一次，内容是："<<endl;
        it->writeToFile(_path);
        //it->printTable();
    }
    return;
}


void CacheManger::print(){
    cout<<"void CacheManger::print()"<<endl;
    for(auto a:_cacheList){
        a.printTable();
        cout<<endl;
    }

    cout<<"_Thread2Cache.size="<<_Thread2Cache.size()<<endl;
}

//暂时估计用不到的功能
unordered_map<string,string> CacheManger::loadFile(){
    //_path=filename;
    ifstream in(_path);//文件流也可以自动创建文件
    string Line;
    unordered_map<string,string> tempCache;
    while (getline(in,Line))
    {
        cout<<"readFromFime:line is: "<<Line<<endl; 
        string indexWord;
        string candidateWord;
        string word;
        istringstream record(Line);
        record>>indexWord;
        while(record>>word){
            cout<<"word is: "<<word<<endl; 
            candidateWord+=word;
        }
        tempCache.insert(make_pair(indexWord,candidateWord));
    }
    return tempCache;
}



unordered_map<string,string> CacheManger::eliminateDuplicate(unordered_map<string,string> indexLine){
    unordered_map<string,string> tempMap=loadFile();
    for(auto i:indexLine){
        auto it=tempMap.find(i.first);
        if(it!=tempMap.end()){
            it->second=i.second;
        }else{
            tempMap.insert(i);
        }
    }
    return tempMap;
    
}

void CacheManger::updateCache(size_t i,Cache c){
    cout<<"void CacheManger::updateCache(size_t i,Cache c)"<<endl;
    _cacheList[i].update(c);
}

}

/*
void testCacheManger(){
    //从同一个文件读入
    const string path="磁盘缓存.txt";
    //cout<<"初始化三个"<<endl;
    wd::CacheManger::initCache(9,path);
    wd::CacheManger::initCache(5,path);
    wd::CacheManger::initCache(8,path);
    //做不同的改变
    wd::Cache c0=wd::CacheManger::getCache(0);
    cout<<"cache0本来是"<<endl;
    c0.printTable();
    wd::Cache c1=wd::CacheManger::getCache(1);
    cout<<"cache1本来是"<<endl;
    c1.printTable();
    wd::Cache c2=wd::CacheManger::getCache(2);
    cout<<"cache2本来是"<<endl;
    c2.printTable();
    
    
    //返回的引用变量，是否可以改变变量的底层值（深复制），是问题的关键
    cout<<endl<<endl<<"插入数据～～～～"<<endl;
    c1.addElement("c1","我就又一次 传进去c1 试试");
    cout<<"cache1之后是"<<endl;
    c1.printTable();
    c2.addElement("c","我就又一次 传进去c1 试试");
    cout<<"cache2之后是"<<endl;
    c2.addElement("c2","我就又一次 传进去c2 试试");
    c2.printTable();



    //查询管理类内部的，属性是否被改变
    cout<<endl<<endl<<"进入新函数，更新容器内元素:"<<endl;
    c1.printTable();
    wd::CacheManger::updateCache(1,c1);
    cout<<"更新c2"<<endl;
    wd::CacheManger::updateCache(2,c2);
    cout<<"查询管理类内部的，属性是否被改变:"<<endl;
    wd::Cache c=wd::CacheManger::getCache(1);
    c.printTable();
    c=wd::CacheManger::getCache(2);
    c.printTable();
    //再写回
    cout<<"写回数据"<<endl;
    wd::CacheManger::periodicUpdate(1);
}
*/

