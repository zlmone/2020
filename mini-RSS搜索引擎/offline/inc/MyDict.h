#pragma once

#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<utility>
using namespace std;


namespace wd {


	
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
		void init(string data);
		void inputString(string data);
	
	
		void printAll();
		map<string, int>& getDict() {
			return _frequency;
		}
		map<string, set<string>>& getIndexTable() {
			return _index_table;
		}

		static void destory() {
			if (_pInstance) {
				delete _pInstance;
			}
		}
	private:
		MyDict() { cout << "mydict" << endl; }
		~MyDict() { cout << "mydict" << endl; }


		void inputArticle(const char* path);
		void insertOrUpdateIndex(string Word);
		void insertOrUpdateFrequency(string Word);
		size_t nBytesCodeInDict(const char ch);


	private:
		static MyDict* _pInstance;
		vector<pair<string, int>> _dict;
		map<string, int> _frequency;
		//map<string, set<int>> _index_table;
		map<string, set<string>> _index_table;
	};
	


}



