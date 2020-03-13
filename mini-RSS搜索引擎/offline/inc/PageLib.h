
#ifndef _PAGELIB_H
#define _PAGELIB_H

#include"Configuration.h"
#include"FileProcessor.h"
#include"DirScanner.h"

#include<vector>
#include<map>
#include<utility>
#include<string>
using namespace std;


namespace wd{

class PageLib {
private:
    Configuration & _conf;
    string _dir;
    DirScanner & _dirScanner;
    FileProcessor & _fileProcessor;
    vector<string> _vecFilesfiles;
    map<int,pair<int,int>> _offsetLib;
    
public: 
    PageLib(Configuration &conf,DirScanner &dirScanner,FileProcessor &fileProcessor);
    //创建网页库
    void create();
    //存储网页库和位置偏移库
    void store();
};

}

#endif //_PAGELIB_H