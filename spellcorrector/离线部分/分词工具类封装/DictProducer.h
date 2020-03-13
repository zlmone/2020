/**
 * Project 基本网络编程工具
 */


#ifndef _DICTPRODUCER_H
#define _DICTPRODUCER_H
#include"SplitToolCppJieba.h"

//开发文档的功能过于强大，你只需要将语料库文件读入，并建立出词典即可
//预料库文件，暂时默认分出两个，一个是中文，一个是英文
//英文文件，直接建立词典即可，中文文件需要进行分词处理之后再建立词典
namespace wd{

class DictProducer {
public: 
    

DictProducer(const string &dir);
    
DictProducer(const string &dir,SplitToolCppJieba *splitTool);
    
void build_dict();
    
void bulid_cn_dict(const string &dir);
    


void store_dict(const string &filename);
void show_dict();
void show_files();
void get_files();
    


void push_dict(const string &word);
protected: 
    
~DictProducer();
private: 
    string _dir;
    vector<string> _files;
    map<string,int> _dict;
    SplitToolCppJieba* _splitTool;
    vector<string> _chineseWareHouse;
};

}

#endif //_DICTPRODUCER_H