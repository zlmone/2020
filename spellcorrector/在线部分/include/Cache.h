

#ifndef _CACHE_H
#define _CACHE_H
#include<unordered_map>
#include<utility>
#include<string>
#include<iostream>
#include"LRUwindow.h"
using namespace std;

namespace wd{

template class LRUwindow<pair<string,string>>;
class Cache {

public: 
Cache(int);                             //传入num有什么用?????????????
Cache(const Cache &cache);//bingo
    
void addElement(const string &key, const string &value);        

//将一个缓存赋值给另一个？？？？？
void update(const Cache &rhs);              
//从文件中读写
void readFromFile(const string &filename);
void writeToFile(const string &filename);
//void writeToFile();
void printTable();
string findTheWord(string s);


private: 
    //unordered_map<string,string> _table;
    LRUwindow<pair<string,string>> _table;      
    //map中的内容：查询词，查询词对应的候选词
    //string _path;//这个我自己加的
};
//成员为内存cache
//另会有传入文件，作为磁盘cache
//两个cache内容，宏观上相同


}


#endif //_CACHE_H