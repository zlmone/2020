#include"../include/MyDict.h"
#include<fstream>
#include<sstream>
#include<algorithm>
using namespace std;


namespace wd {
	void MyDict::init(const char* dictEnPath) {
		inputArticle(dictEnPath);
		cout << "void init(const char* dictEnPath)" << endl;
	}



//读入文件，包含大写换小写功能
	void MyDict::inputArticle(const char* path) {
		//cout << "int indexMachine::inputArticle(string path)" << endl;
		ifstream in(path);
		if (!in) {
			cout << "error file or not exist" << endl;
			return;
		}
		cout << "indexMachine::inputArticle(string path)" << endl;


		string Line;						
		string word;					    
		//int i = 0;
		while (getline(in, Line)) {		
			//lines.push_back(Line);		
			//printf("%d\r", i);			
			//printf("%d ", i);
			//i++;
			istringstream record(Line);		
			//Line.erase(Line.find(( '.') || (',') || ('?') || ('!') || ( ':') || ( ';')))
			cout << endl << "temp line is " << Line << endl;


			while (record >> word) {
				//cout << word << endl;
				//isFind = 0;

				bool isEnglish = 1;
				//cout << "temp word is " << word << "->";
				//对每个字母进行处理
				for (auto it = word.begin(); it != word.end(); ++it) {
					//cout << *it << "->";
					*it = tolower(*it);
					if (((*it) - 'a' < 0) || ((*it) - 'a' > 25)) {
						cout << "find a letter no english" << *it<<endl;
						if (((*it) == '.') || ((*it) == ',') || ((*it) == '?') || ((*it) == '!') || ((*it) == ':') || ((*it) == ';')) {
							//标点符号，应该被算作下一个词了
							break;
						}
						else
						{
							isEnglish = 0;
							break;
						}
					}
				}
				/*
				if (isEnglish == 0) {
					continue;
				}*/
				//cout << endl;
				//cout << "final word is " << word <<endl;



				//ȥ���Ż���
				string temp;
				auto it = word.end();
				auto c = *(it - 1);
				//去掉标点
				if ((c == '.') || (c == ',') || (c == '?') || (c == '!') ||
						 (c == ':') || (c == ';')||(c=='[')||(c==']')) {
					temp.resize(word.size() - 1);
					//cout << "�б��";
					for (size_t i = 0; i != word.size() - 1; ++i) {
						temp[i] = word[i];
					}
				}
				else {
					temp = word;
				}
				insertOrUpdateFrequency(temp);
				vector<string> tempVs;
				if(isEnglish==1){
					insertEnglishIndex(temp);
				}else
				{
					tempVs.push_back(temp);
				}
				insertChineseIndex(tempVs);	
			}
		}
	}



	void MyDict::insertOrUpdateFrequency(string Word) {
		//cout << "void MyDict::insertOrUpdateFrequency(string Word)" << endl;
		if (_frequency.find(Word) == _frequency.end()) {
			_frequency.insert(make_pair(Word, 1));
		}
		else {
			_frequency[Word]++;
		}
		//cout << "temp is " << Word;
		//cout << endl;
	}


	void MyDict::printAll() {
		cout << "void Dictionary::printAll()" << endl;
		cout <<endl<< "词语个数:" << _frequency.size() << endl;
		for (auto item : _frequency) {
			cout << item.first << " " << item.second << endl;
		}
		cout <<endl<< "索引个数:" << _index_table_Chinese.size() << endl;
		for (pair<string, set<string>> item : _index_table_Chinese) {
			cout << item.first << ":";
			for (auto w : item.second)
			{
				cout << w << " ";
			}
			cout << endl;
		}
		cout << "mission success" << endl;
	}



	void MyDict::insertEnglishIndex(vector<string> word){
		_index_table_English.build2(word);

	}

	//基本项目实现v2引入中文处理功能之后，需要修改索引
	//从这里就需要，对中文进行偏移处理
	void MyDict::insertChineseIndex(string word) {
		//cout<<" void MyDict::insertOrUpdateIndex(string Word)"<<endl;
		string subAlp;
		for (size_t idx=0;idx<=word.size();++idx) {
			//cout<<i<<" ";
			size_t nBytes=nBytesCodeInDict(word[idx]);
			subAlp=word.substr(idx,nBytes);
			//subAlpUtf=subAlp.c_str();
			//lhs_idx+=nBytes-1;
			idx+=nBytes-1;
			//cout<<"nbytes="<<nBytes<<" ";
			//cout<<"sublhs ="<<subAlp<<endl;

			if (_index_table.find(subAlp) == _index_table.end()) 
			{
				set<string> tempWord;
				tempWord.insert(word);
				_index_table.insert(make_pair(subAlp, tempWord));
				continue;
			}
			else {		
				_index_table[subAlp].insert(word);
			}
		}
		cout<<endl;
	}

//后面可以设置成友元，以减小代码量
	size_t MyDict::nBytesCodeInDict(const char ch){
		//进行按位与操作，如果此字节的字符首位为1，则会进入if条件
		if(ch & (1<<7)){
			int nBytes=1;
			for(int idx=0;idx!=6;++idx){
				if(ch & (1<<(6-idx))){
					++nBytes;//用于计算这个字符所占位数（UTF-8）
				}else{
					break;
				}
			}
			return nBytes;
		}
		//说明这个字符首位不是1，也就说，这个字节是个字母
		return 1;
	}
}








