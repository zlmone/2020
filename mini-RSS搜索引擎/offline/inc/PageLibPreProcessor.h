#ifndef _PAGELIBPREPROCESSOR_H
#define _PAGELIBPREPROCESSOR_H
#include"Configuration.h"
#include"SplitToolCppJieba.h"
#include"WebPage.h"
#include<vector>
#include<unordered_map>
#include<utility>
#include<string>
#include<map>
#include"PageLib.h"
using namespace std;


namespace wd{

class PageLibPreProcessor { 
    Configuration _conf;
    map<string,string> _confMap;
    SplitToolCppJieba _jieba;           //bingo
    vector<WebPage> _vecWebPage;        //bingo
    unordered_map<int,pair<int,int>> _offsetLib;    //bingo
    unordered_map<string,vector<pair<int,double>>> _invertIndexTable;
    //            单词，候选文档集合（文档id，文档权重值）

public: 
    PageLibPreProcessor(Configuration &conf)
    :_conf(conf)
    {
        
    }

    //预处理,暂时认为只是用来调用私有函数的                                        封装
    void doProcess();

//private:
    //1、根据配置信息读入网页库和偏移库的内容                       写完了
    void readInfoFromFile();
    //2、网页去重                                              写完了
    void cutRedundantPaged();
    //3、建立倒排索引                                           难点
    void buildInvertIndexTable();
    //4、将经过预处理的网页库，网页偏移库，倒排索引表，写回磁盘        写完了
    void storeOnDisk();
};

}

#endif //_PAGELIBPREPROCESSOR_H