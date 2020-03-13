/**
 * Project 基本网络编程工具
 */

#pragma once
#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H


#include<string>
#include<map>
using namespace std;

namespace wd{
    
class Configuration {
public: 
    
//读入配置文件，需要时再加入项目
Configuration(const string &filepath);
void printMap();
map<string,string> & getConfigMap();
//可以用于修改内容


private: 
    string _filepath;
    map<string,string> _configMap;
};

}

#endif //_CONFIGURATION_H