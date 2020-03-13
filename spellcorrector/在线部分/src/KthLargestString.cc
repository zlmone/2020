#include "../include/KthLargestString.h"
#include"../include/MyDict.h"


#include<iostream>
#include<string>
#include<map>
#include<utility>
#include<vector>
#include<algorithm>
using namespace std;


namespace wd {
	int distM[1000][1000];
	KthLargestString::KthLargestString(int num, map<string, int>& msi,map<string,set<string>> index)
		:_size(num)
		, _tempMsi(msi)
		,_wordIndex(index)
	{
		
		auto it = _tempMsi.begin();
		while (it != _tempMsi.end())
		{
			_data.push_back(it->first);
			++it;
		}
	}
	vector<string>  KthLargestString::retHeap(string word) {
		cout << "retHeap(string word):_data.size()=" << _data.size() << endl;
		mySort(word);;
		vector<string> ret;
		if (_size > _data.size()) {
			ret= _data;
		}
		else
		{
			auto it = _data.begin();
			int len = (_size < _data.size() ? _size : _data.size());
			cout << "len=" << len << endl;
			for (auto i = 1; i != len+1; ++i) {
				cout << "_data.size() is " << _data.size() << endl;
				cout << "_data.size() - i is: " << (_data.size() - i) << endl << endl;
				ret.push_back(_data[_data.size() - i]);
				++it;
			}
		}
		updateFrequency(word);
		return ret;
	}






	void KthLargestString::mySort(string word) {
		if (_data.size() < _size)
		{
			//���д�С�����޶��Ĵ�С��С�������õ��ѣ�ֱ���������
			//heapArrange(0, _data.size() - 1);
			//cout << "_data.size()="<<_data.size()<<" is too little,just cout" << endl;
		}
		else
		{
			cout << "mySort:large enough" << endl;
			//�������(����⣩������Ҫ����k�Σ�k���ѵĴ�С_size
			for (size_t i = 1; i < _size+1; ++i) {
				heapArrange(0, _data.size() - i,word);
				//�ڵ�һ�ε�����ȫ��ɺ󣬵���swap���º�
				swap(_data[0], _data[_data.size() - i]);
			}
			/*�ϸ�
			if (compareWord(val, _data[_data.size() - _size]) {
				_data.push_back(val);
					heapArrange(_data.size() - _size, _data.size() - 1);
			}
			return _data[_data.size() - _size];*/
		}
		return;
	}
	//�ڶ����б�׼:���´�Ƶ
	void KthLargestString::updateFrequency(string word) {
		//cout << "void KthLargestString::updateFrequency(string word)" << endl;
		if (!_tempMsi[word]) {
			_tempMsi.insert(make_pair(word, 0));
			_data.push_back(word);
		}
		else {
			_tempMsi[word]++;
		}
	}







	//endԭ����Ƶ���β������
	void KthLargestString::heapArrange(int start, int end, string word) {
		//cout << "heapArrange()"<<endl;
		int leave = end;
		while (leave > start+1)
		{
			adjust(start, leave, word);
			leave--;
		}
		cout << "heapArrange(int start, int end, string word) is over" << endl << endl;
	}
	void KthLargestString::adjust(int startd, int endd, string word) {
		int m = endd - startd-1;
		//cout << "adjust():m + startd=		   " << m + startd << endl
		//	 << "adjust():(m - 1) / 2 + startd=" << (m - 1) / 2 + startd << endl;
		//cout << "_data[m + startd]=" << _data[m + startd] << endl
			//<< "_data[(m - 1) / 2 + startd]=" << _data[(m - 1) / 2 + startd] << endl;

		while (compareWord(_data[m + startd], _data[(m - 1) / 2 + startd],word)) {
			swap(_data[m + startd],_data[(m - 1) / 2 + startd]);
			if ((m - 1) / 2 == 0) {
				break;
			}
			m = (m - 1) / 2;
		}
		return;
	}


















	bool KthLargestString::compareWord(string word1, string word2, string word) {
		//cout << "compare word():" << endl;
		
		if (minDistance(word1, word) > minDistance(word2, word)) {
			return false;
		}
		else if (minDistance(word1, word) < minDistance(word2, word)) {
			return true;
		}
		else {
			if (_tempMsi[word1] > _tempMsi[word2]) {
				return false;
			}
			else if (_tempMsi[word1] > _tempMsi[word2]) {
				return true;
			}
			else {
				return compareAlphabeticalOrder(word1, word2);
			}
		}
	}



//备忘录数组直接使用的自己预先设定的大小，但是这样可能不太好，重构时要想办法
	int KthLargestString::minDistance(string word1, string word2) {	
		//cout << "minDistance(string word1, string word2)" << endl;
		const int len1 = word1.length();
		const int len2 = word2.length();
		int min = 0;
		//int distM[10][10];

		for (size_t row = 0,i=0; i < word1.length()+1; ++i,++row) {
			size_t nBytes=nBytesCode(word1[i]);
			i+=nBytes-1;
			distM[row][0] = row;
		}
		for (size_t col = 0,i=0; i < word2.length()+1; ++i,++col) {
			size_t nBytes=nBytesCode(word2[i]);
			i+=nBytes-1;
			distM[0][col] = col;
		}
		
		
		for (size_t i = 1; i <= word1.length(); ++i) {
			for (size_t j = 1; j <= word2.length(); ++j) {
				distM[i][j] = distM[i][j - 1] + 1;
				if (distM[i - 1][j] + 1 < distM[i][j])
				{
					distM[i][j] = distM[i - 1][j] + 1;
				}
				
				min = (word1[i - 1] == word2[j - 1] ? distM[i - 1][j - 1] : distM[i - 1][j - 1] + 1);
				if (min < distM[i][j])
				{
					distM[i][j] = min;
				}
			}
		}
		return distM[len1][len2];
	}
	

	//字母序比较,引入中文之后，暂时也不改，
	//尽管中文没有字母序，但是字符表中也是有顺序的，可以作为比较依据
	bool KthLargestString::compareAlphabeticalOrder(string word1, string word2) {
		//cout << "bool KthLargestString::compareAlphabeticalOrder(string word1, string word2)" << endl;
		const int len1 = word1.length();
		const int len2 = word2.length();
	
		for (int i = 1; i != word1.length(); ++i) {
			for (int j = 1; j != word2.length(); ++j) {
				if (word1[i] < word2[j])
				{
					return true;
				}
				else if (word1[i] > word2[j])
				{
					return false;
				}
			}
		}
		if (len1 < len2) {
			return true;
		}
		else
		{
			return false;
		}
	}


//基本项目功能实现v2版本添加
	size_t KthLargestString::nBytesCode(const char ch){
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

/*
namespace wd {
	MyDict* MyDict::_pInstance = nullptr;
}
*/


/*
int test(void) {
	//事先初始化一个字典
	wd::MyDict* p1 = wd::MyDict::createInstance();
	p1->printAll();
	cout << "初始化之前" << endl << endl << endl;
	p1->init("圣经第一章.txt");
	cout << "初始化之后" << endl << endl << endl;
	p1->printAll();


	//将字典传入选词类
	map<string, int> initMsi = p1->getDict();
	wd::KthLargestString myTask(3, initMsi);
	vector<string> vs;
	string word;
	cout << "请输入要查询的单词:";
	while (cin >> word)
	{
		cout << endl;
		vs = myTask.retHeap(word);
		for (auto i : vs) {
			cout << i << " ";
		}
		cout << "请输入要查询的单词:";
	}
	return 0;
}*/
