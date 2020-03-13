#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<utility>
#include<sstream>
using namespace std;

/*
	byte
	bytedance
	bytecoin
	bytespeed

	*/

class Trie
{
	



	//˼·��ʹ�ö�Ԫ��������ֵ����Ľ���
public:
	/*Trie(vector<string> vs) {
		init();
		build2(vs);
	}*/
	Trie() 
		:_isEnd(false)
	{
		init();
	}
	~Trie() {}
	vector<string> find2(string str);
	//���ҹ�����ʱ��֧������
	//���Ը�����Ŀ��Ҫ�Լ�д�������󣬲�Ȼ��һ����Ҫ��ĸ������
	vector<string> search();
	vector<string> getPossible(string str);
	void print2();
	void build2(vector<string> vs);

#if 1
	void debug() {

	}
	vector<string> _ret;
#endif

private:
	
	
	void init();

	bool _isEnd;
	vector<pair<char, Trie*>> _childs;
	//vector<Trie*> _pointers;
	//map<string, Trie*>_childs;
};










//	����
/*bytedance
	ali
	tecent*/

	//	��ѯ����find2
	//	byte
	//	alipay
	//	tecend
	/*
	void test2() {
		vector<string> input;
		string s;
		while (cin >> s)
		{
			input.push_back(s);
		}

		Trie T;
		//T.print2();
		T.build2(input);
		T.print2();

		cout << "��ʼ��ѯ:" << endl;
		//string str;
		//cin >> str;
		string out = T.find2("byte");
		cout << "���������" << out;

		out = T.find2("alipay");
		cout << "���������" << out;

		out = T.find2("tecend");
		cout << "���������" << out;

	}*/


