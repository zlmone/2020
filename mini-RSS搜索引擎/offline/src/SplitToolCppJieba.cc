/**
 * Project 基本网络编程工具
 */


#include "../inc/SplitToolCppJieba.h"
#include<iostream>
#include<vector>
#include<string>
using namespace std;
/**
 * SplitToolCppJieba implementation
 */
namespace wd{



const char* const DICT_PATH = "../cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "../cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../cppjieba/dict/stop_words.utf8";


SplitToolCppJieba::SplitToolCppJieba()
:_jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH,IDF_PATH,STOP_WORD_PATH)
{}

SplitToolCppJieba::~SplitToolCppJieba() {
    ;
}


vector<string> SplitToolCppJieba::cut(const string &sentence)  {
    vector<string> words;
    cout<<"[demo] cut with HMM"<<endl;
    _jieba.Cut(sentence,words,true);
    for(auto w:words){
        cout<<w<<" ";
    }
    cout<<endl;
    cout<<"[demo] cut without HMM"<<endl;
    _jieba.Cut(sentence,words,false);
    for(auto w:words){
        cout<<w<<" ";
    }
    cout<<endl;
    return words;
}

}

