/**
 * Project 基本网络编程工具
 */


#ifndef _SPLITTOOLCPPJIEBA_H
#define _SPLITTOOLCPPJIEBA_H

#include"SplitTool.h"
#include "../cppjieba/include/cppjieba/Jieba.hpp"
#include<vector>
#include<string>
using namespace std;


namespace wd{

class SplitToolCppJieba
//: public SplitTool
{
public: 
    
SplitToolCppJieba();    
~SplitToolCppJieba();
vector<string> cut(const string &sentence);

private: 
    //Configuration & _conf;
    cppjieba::Jieba _jieba;

};

}



#endif //_SPLITTOOLCPPJIEBA_H