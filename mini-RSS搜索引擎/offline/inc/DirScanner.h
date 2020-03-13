/**
 * Project 基本网络编程工具
 */


#ifndef _DIRSCANNER_H
#define _DIRSCANNER_H
#include"Configuration.h"
#include<string>
#include<vector>
using namespace std;

//conf中会保存，语料文件的目录的路径
//本类可以在目录中循环读取，每一个语料文件的文件名
namespace wd{


//传入目录，传出所有文件的文件名向量
//注意，你必须事先保证传入的目录内，所有文件合法，且都是xml文件，否则会崩。。。。。。。。。。。。。。。。。。。。。。。重构预警
//重构时，要添加判断文件合法性的功能，必须是".xml"文件
class DirScanner {
private:
    vector<string> _vecFilesfiles;
    Configuration & _conf;
    string _dirPath;//暂时不用，获取目录路径，暂时直接传参进来即可
                    //就是不知道配置文件类，作为成员有什么用了
public: 
    DirScanner(Configuration &conf);
    
    void operator()(const string &name);
    //返回_vecFilesfiles的引用
    vector<string> files();

private:
    void traverse(const string &dirname);
    //获取某一目录下所有文件
};

}

#endif //_DIRSCANNER_H