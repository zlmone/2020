#ifndef _FILEPROCESSOR_H
#define _FILEPROCESSOR_H
#include"Configuration.h"
#include<string>
using namespace std;



namespace wd{

class FileProcessor {
private:
    string _titleFeature;
    Configuration& _conf;


public: 
    FileProcessor(Configuration &conf)
    :_conf(conf)
    {}
    //处理一篇具体文档，返回一个长字符串
    string process(int &idx,const string &filename);
    //获取一篇文档的标题和内容
    void readfile(const string &filename,string &title,string &content,string &urlAddr);
};

}

#endif //_FILEPROCESSOR_H