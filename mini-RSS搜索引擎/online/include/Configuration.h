/**
 * Project 基本网络编程工具
 */

#pragma once
#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H


#include<string>
#include<map>
#include<utility>
#include<iostream>
#include<set>
using namespace std;

namespace wd{
    
class Configuration {
public: 
    
//读入配置文件，需要时再加入项目
Configuration(const string &filepath);
void printMap();
map<string,string> & getConfigMap();
//可以用于修改内容
pair<size_t,size_t> getThreadPoolConf(){
    cout<<"调试次数10"<<endl;
    auto it1=_configMap.find("ThreadNumber");
    auto it2=_configMap.find("TaskNum");
    cout<<"it1->second= "<<it1->second<<endl;
    cout<<"it2->second= "<<it2->second<<endl;

    int b=stoi(it2->second);
    cout<<"b="<<b<<endl;
    int a=atoi((it1->second).c_str());
    cout<<"a="<<a<<endl;
    return make_pair(a,b);
}

pair<size_t,size_t> getTimerConf(){
    cout<<"pair<size_t,size_t> getThreadPoolConf()"<<endl;
    auto it1=_configMap.find("写缓存启动时间");
    cout<<"pair<size_t,size_t> getThreadPoolConf()"<<endl;
    auto it2=_configMap.find("写缓存轮写周期");
    return make_pair(stoi(it1->second),stoi(it2->second));
}

pair<const string,unsigned short> getNetConf(){
    cout<<"pair<size_t,size_t> getThreadPoolConf()"<<endl;
    auto it1=_configMap.find("ip_address");
    cout<<"pair<size_t,size_t> getThreadPoolConf()"<<endl;
    auto it2=_configMap.find("port");
    return make_pair(it1->second,stoi(it2->second));
}
private: 
    void readConfiguration();
    string _filepath;
    map<string,string> _configMap;
    set<string> stopWOrdList;
};

}

#endif //_CONFIGURATION_H