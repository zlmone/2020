/**
 * Project 基本网络编程工具
 */


#include "../inc/DirScanner.h"
//#include"Configuration.cpp"//整合时要注释掉
#include<sys/types.h>
#include<dirent.h>
#include<iostream>
#include<string.h>
using namespace std;



namespace wd{


DirScanner::DirScanner(Configuration &conf)
:_conf(conf)
{
    /*map<string,string> mss=conf.getConfigMap();
    auto ret=mss.find("语料文件路径");
    _dirPath=ret->second;*/
    
}

void DirScanner::operator()(const string &name) {
    traverse(name);
}

vector<string> DirScanner::files() {
    cout<<"DirScanner::files()"<<endl;
    return _vecFilesfiles;//我倒是觉得是否返回引用是无所谓的，不需要改变它的值
}

void DirScanner::traverse(const string &dirname){
    cout<<"输入路径"<<dirname.c_str()<<endl;
    DIR *pDir=opendir(dirname.c_str());
    if(NULL==pDir){
        perror("openDir Error");
        return;
    }
    
    
    struct dirent *pDirInfo;
    while ((pDirInfo=readdir(pDir))!=NULL)
    {
        //strcmp相等则返回0，用于去除本地符号".",以及上层符号".."
        if((strcmp(pDirInfo->d_name,"..")==0)||(strcmp(pDirInfo->d_name,".")==0)){
            //cout<<pDirInfo->d_name<<endl;
            continue;
        }
        cout<<pDirInfo->d_name<<endl;
        _vecFilesfiles.push_back(pDirInfo->d_name);
    }
    closedir(pDir);
}

}


void testDirScanner(){
    wd::Configuration conf("../conf/conf.txt");
    wd::DirScanner dirScanner(conf);

    dirScanner("../data/rss源语料/");
    vector<string> vs=dirScanner.files();

    cout<<endl<<endl<<endl;
    for(auto s:vs){
        cout<<s<<endl;
    }
}

/*
int main(void){
    testDirScanner();
}*/