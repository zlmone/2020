#include "../include/Cache.h"
//open的头文件
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
//read的头文件
#include<unistd.h>
#include<fstream>
#include<sstream>
using namespace std;

#include<iostream>



namespace wd{

Cache::Cache(int size=10)
:_table(size)
{
    cout<<"Cache:cache init success"<<endl;
}

Cache::Cache(const Cache &cache) 
:_table(cache._table)
{} 



void Cache::addElement(const string &key, const string &value) {
    _table.add(make_pair(key,value));
    return;
}


//法一：全盘赋值传入的缓存
//法二、两个缓存合并
void Cache::update(const Cache &rhs) {
    auto it=rhs._table._que.begin();
    cout<<"update before"<<endl;
    for(;it!=rhs._table._que.end();++it){
        cout<<it->first<<" "<<it->second<<endl;
        _table.add(*it);
    }
    return;
}


//cache模块v2在数据值之间加了空格，同时加入去重功能
void Cache::readFromFile(const string &filename) {
    const char *pathname=filename.c_str();
    //_path=filename;
    ifstream in(pathname);//文件流也可以自动创建文件
    string Line;
    while (getline(in,Line))
    {
        //cout<<"readFromFime:line is: "<<Line<<endl; 
        string indexWord;
        string candidateWord;
        string word;
        istringstream record(Line);
        record>>indexWord;
        while(record>>word){
           // cout<<"word is: "<<word<<endl; 
            candidateWord+=" ";
            candidateWord+=word;
        }
        _table.add(make_pair(indexWord,candidateWord));
    }
    cout<<"cache:readFromFile():_table.size()="<<_table.size()<<endl;
    return;
}
void Cache::printTable(){
    cout<<"printTable()"<<endl;
    _table.print();
}

//ofstream可能会抹去文件之前的所有内容
void Cache::writeToFile(const string &filename) {
    ofstream out(filename,ofstream::app);
    string line;
    for(auto a:_table._que){
        line=a.first+" "+a.second;
        out<<line;
        out<<"\n";
    }
    //两个cache之间空两行
    out<<"\n";
    out<<"\n";
    return;
}
//这个是我自己加的
/*void Cache::writeToFile() {
    ofstream out(_path);
    string line;
    for(auto a:_table){
        line=a.first+" "+a.second;
        out<<line;
        out<<"\n";
    }
    return;
}*/


string Cache::findTheWord(string s){
    cout<<"Cache::findtheWord():你要找的是不是 "<<s<<endl;
    bool flag=false;
    string ret="";
    for(auto a:_table._que){
        if(a.first==s){
            cout<<"Cache::findtheWord():我找到的是 "<<a.second<<endl;
            ret=a.second;
            flag=true;
        }
    }
    if(flag==true){
        cout<<"找不到了"<<endl;
    }
    return ret;

    
    /*
    if(it!=_table.end()){
        return it->second;
    }else{
        return "";
    }
    */
}

}

void testCache(){
    wd::Cache cache1;
    wd::Cache cache2(cache1);
    cache1.readFromFile("临时缓存1.txt");
    cout<<endl<<"cache1插入前："<<endl;
    cache1.printTable();
    cache1.addElement("mother","fucker you one");
    cache1.addElement("father","to your fate");
    cout<<endl<<"cache1插入后："<<endl;
    cache1.printTable();
    cache1.writeToFile("临时缓存1.txt");
    cout<<endl<<endl;


    cache2.readFromFile("临时缓存2.txt");//这个文件不存在，让它自己建立
    cache2.update(cache1);
    cout<<endl<<endl<<"cache2插入后："<<endl;
    cache2.printTable();
    cache2.writeToFile("临时缓存2.txt");
}


