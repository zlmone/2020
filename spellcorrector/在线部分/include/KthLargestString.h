#pragma once

#include<iostream>
#include<string>
#include<map>
#include<utility>
#include<vector>
#include<algorithm>
#include<set>
using namespace std;



namespace wd {

	class KthLargestString {
	public:
		
		KthLargestString(int k, map<string, int>& msi,map<string, set<string>> index);
		vector<string> retHeap(string word);


	private:
		void mySort(string);

		void heapArrange(int startd, int endd, string word);
		void adjust(int start, int end, string word);
		size_t nBytesCode(const char ch);
		set<string> findYouWant(string alp);




	private:
		bool compareWord(string word1, string word2, string word);

		//编辑距离算法，中文编码引进时修改
		int minDistance(string word1, string word2);
		void updateFrequency(string word);				
		void updateIndex(string alpIn,string wordIn);														
		bool compareAlphabeticalOrder(string word1, string word2);

	private:
		vector<string> _data;	   //这个是排序好的词库
											
		map<string, int> _tempMsi;//词频类，有空改个名		
		map<string, set<string>> _wordIndex;
		size_t _size;

	};








}






