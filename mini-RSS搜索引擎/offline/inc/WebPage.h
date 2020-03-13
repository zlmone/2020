/**
 * Project 基本网络编程工具
 */

#pragma once
#ifndef _WEBPAGE_H
#define _WEBPAGE_H


#include<string>
#include<map>
#include<set>
#include<vector>
#include"../../tinyxml2/tinyxml2.h"
#include"Configuration.h"
#include"SplitToolCppJieba.h"
using namespace std;



namespace wd{
    //暂时的功能是，传入xml文件路径，解析其内容
    //后续解析功能可能，要移植到别的类

    //还存在的问题
    //      甲、docId怎么搞
    //      乙、构造传入可能要改成字符串，而不是路径
    //      丙、operator<需要用到的是，搜索引擎的业务逻辑，可能要分别和，传进来的搜索要求进行比较

class WebPage {
private: 
    const static int TOPK_NUMBER;
    string _doc;        //bingo
    int _docId;         //bingo
    string _docTitle;   //bingo
    string _docUrl;     //bingo
    string _docContent; //bingo
    string _docSummary;
    vector<string> _topWords;
    map<string,int> _wordsMap;
    //tinyxml2::XMLDocument *_xmlDoc;
    tinyxml2::XMLElement *_xmlElem; //bingo
public:
    //构造，并且解析字符串                                  //？？？？？？？改完没有调试
    WebPage(string &doc,Configuration &config,SplitToolCppJieba &jieba);                      
    ~WebPage(){}
    
    int getDocId(){
        return _docId;
    } 
    string getDoc(){
        return _doc;
    }
    //左值可以作为右值，但是右值不能作为左值
    map<string,int>& getWordsMap(){
        return _wordsMap;
    }
    vector<string>& getWords(){
        return _topWords;
    }
        string getDocTitle(){
        return _docTitle;
    }
    string getDocContent(){
        return _docContent;
    }
    string getUrl(){
        return _docUrl;
    }


    //功能需求：对格式化文档进行处理
    //个人观点：分词，提取停用词集，调用calcTopK
    //问题：doc干嘛用的？？
    void processDoc(const string &doc,Configuration &config,SplitToolCppJieba &jieba);
    
    //功能需求：求取topK词集
    //个人观点：传入一个分好词的词库，统计词频和主要词
    void calcTopK(vector<string> &wordsVec,int k,set<string> &stopWordList);


    friend bool operator==(const WebPage &lhs,const WebPage &rhs);                      //写完，没调试
    friend bool operator<(const WebPage &lhs,const WebPage &rhs);                       //不知道用来干嘛？？？？？？？？？
};


}


#endif //_WEBPAGE_H