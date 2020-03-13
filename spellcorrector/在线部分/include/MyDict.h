#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<utility>
#include"Trie.h"
using namespace std;


namespace wd {

//v8版本添加新的insertEnglish函数
//返回索引的函数getIndexTable，也需要修改
//		合并两个索引，然后再返回
	
	class MyDict
	{
	public:
		static MyDict* createInstance() {
			if (nullptr == _pInstance) {
				_pInstance = new MyDict();
			}
			return _pInstance;
		}
		void init(const char* dictEnPath);

	
	
		void printAll();
		map<string, int>& getDict() {
			return _frequency;
		}
		map<string, set<string>>& getIndexTable() {
			


			return _index_table_Chinese;
		}

		static void destory() {
			if (_pInstance) {
				delete _pInstance;
			}
		}
	private:
		MyDict() { cout << "mydict" << endl; }
		~MyDict() { cout << "mydict" << endl; }


		void inputArticle(const char* path);//调用后面两个
	
		void insertChineseIndex(string word);
		void insertEnglishIndex(vector<string> Word);
		void insertOrUpdateFrequency(string Word);
		size_t nBytesCodeInDict(const char ch);


	private:
		static MyDict* _pInstance;
		vector<pair<string, int>> _dict;
		map<string, int> _frequency;
		//map<string, set<int>> _index_table;
		map<string, set<string>> _index_table_Chinese;
		Trie _index_table_English;
	};



}



