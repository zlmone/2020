/**
 * Project 基本网络编程工具
 */


#ifndef _WORDQUERY_H
#define _WORDQUERY_H
#include<string>
#include<set>
//#include<unordered_map>
#include<vector>
#include<utility>

#include"WordSegmentation.h"
#include"Configuration.h"
#include"WebPage.h"
#include<map>
using namespace std;



namespace wd{


class WordQuery {
protected:     
    //WordQuery();

public: 
    WordQuery(Configuration &conf):_conf(conf){
        loadLibrary();
    }

    //查询，返回结果
    string doQuery(const string &str);    

private:
    //1.加载库文件
    void loadLibrary();
    //2.计算查询词权重
    vector<double> getQueryWordsWeightVector(vector<string> &queryWords);    
    //3.执行查询,为查询到的结果进行排序
    bool excuteQuery(const vector<string> &queryWords,vector<pair<int,vector<double>>> &resultVec);
    //4.封包
    string createJson(vector<int> &docIdVec,const vector<string> &queryWords);
    //5.查不到，返回
    string returnNoAnswer();
    void dataPrint();


private:
	vector<int> cosSimilar(vector<pair<int, vector<double>>> &input,vector<double> Query);

private: 
    Configuration _conf;
	WordSegmentation _wordSeg;
	map<unsigned int,WebPage> _pagelib;
    unordered_map<int,pair<int,int>> _offsetLib;
    /*std::unordered_map<string,set<pair<int,double>>>
                         _invertIndexTable;*/
    unordered_map<string,map<int,double>>
                         _invertIndexTable;                     
    //倒排索引表
    vector<double> _search;

    map<int,double> _sum;//保存查找结果
};


}

#endif //_WORDQUERY_H