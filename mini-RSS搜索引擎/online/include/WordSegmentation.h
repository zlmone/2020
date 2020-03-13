 ///
 /// @file    WordSegmentation.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2016-01-18 15:43:06
 ///

#ifndef _WD_WORD_SEGMENTATION_H_
#define _WD_WORD_SEGMENTATION_H_


#include "cppjieba/Jieba.hpp"
#include<fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace wd
{

const char * const DICT_PATH = "/home/qianqi/cpp项目：搜索引擎/搜索引擎在线部分2.4/inc/cppjieba/dict/jieba.dict.utf8";
const char * const HMM_PATH = "/home/qianqi/cpp项目：搜索引擎/搜索引擎在线部分2.4/inc/cppjieba/dict/hmm_model.utf8";
const char * const USER_DICT_PATH = "/home/qianqi/cpp项目：搜索引擎/搜索引擎在线部分2.4/inc/cppjieba/dict/user.dict.utf8";
const char* const STOP_WORD_PATH = "../dict/stop_words.utf8";

class WordSegmentation
{
public:
	WordSegmentation()
		: _jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH)
	{
		cout << "cppjieba init!" << endl;
	}


	vector<string> operator()(const char * src)
	{
		cout<<"WordSegmentation::operator()()"<<endl;
		cout<<src<<endl;
		vector<string> words;
		_jieba.CutAll(src, words);//cut with hmm
		cout<<"打印"<<endl;
		for(auto s:words){
        	cout<<s<<" ";
    	}
    	cout<<endl;
		return words;
	}

	vector<string> getRidOfStopwords(vector<string> &vs) {
		cout<<"WordSegmentation::getRidOfStopwords()"<<endl;
		ifstream input(STOP_WORD_PATH);
		string line;
		set<string> stopWords;
		while (getline(input,line))
		{
			stopWords.insert(line);
		}
		
		for (auto it = vs.begin(); it != vs.end();) {
			if (stopWords.find(*it) != stopWords.end()) {
				it = vs.erase(it);
			}
			else {
				++it;
			}
		}
		return vs;
	}
private:
	cppjieba::Jieba _jieba;
};

}// end of namespace wd


#endif
