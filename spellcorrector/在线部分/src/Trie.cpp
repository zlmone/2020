#include"v2.h"
#include<deque>
#include<utility>
using namespace std;
//原创版本的思路是，每次建立一个点，然后从底到顶来链接
//可能的错误在于
//	甲、指针的建立必须分配空间，不然不知道会怎么样
				//也就是说，最好事先设置好一个节点有多少个索引
//		要么可能就是要设置空间的动态增长
//		使用堆进行存储（即用new来申请空间）
//	乙、顺序可能是要从头往后


void Trie::init() {
	_childs.resize(26);
	for (size_t i = 0; i < 26; ++i) {
		this->_childs[i].first = 'a' + i;
		this->_childs[i].second = nullptr;
	}
}


//你根本不知道怎么给个指针分配内存，写不出来的
void Trie::build2(vector<string> vs) {
	//每个单词为一轮，构造到底
	cout << "void Trie::build()构建前 " << endl;
	for (auto s:vs) {
		Trie* temp=this;
		for (size_t i = 0;i!=s.size();++i) {
			Trie t;
			//建立到顶
			if (s[i]>='a'&&s[i]<='z') {
				if (temp->_childs[s[i] - 'a'].second==nullptr) {
					temp->_childs[s[i] - 'a'].second =new Trie(t);
				}
				temp->_isEnd = false;
				temp = temp->_childs[s[i] - 'a'].second;
			}
			else
			{
				cout << "暂不支持非英语言" << endl;
				goto end;
			}

		}
		temp->_isEnd = true;
	end:
		cout << "下一轮" << endl;
	}
	


}


/*
void Trie::build(vector<vector<string>> vvs) {
	//每个单词为一轮，构造到底
	cout << "void Trie::build() " << endl;
	for (auto vs : vvs) {
		Trie child;
		child._childs.insert(make_pair(vs[vs.size() - 1], nullptr));
		Trie* son = &child;
		//不能是空指针，后面要往里面插东西


	   //每个字，构造一个节点

		for (size_t s = vs.size() - 2; s >= 0; --s) {
			Trie temp;
			//建立到顶
			if (s == 0) {
				_childs.insert(make_pair(vs[s], son));

				break;
			}
			else
			{
				//cout << "判断son" << endl;
				if (son == nullptr) {
					cout << "son是个空指针" << endl;
				}
				else {
					for (auto a : son->_childs) {
						cout << a.first << " " << a.second << endl;
					}
				}
				//插入的时候给崩了,看上去不能向空指针中插入数据
				(temp._childs).insert(make_pair(vs[s], son));
				Trie* tempSon = &temp;
				son = tempSon;		//写成son=&temp也不知道对不对
			}

		}
	}


}*/


void Trie::print2() {
	Trie temp = *this;
	if (_isEnd == true) {
		return;
	}
	for (auto a : temp._childs) {
		if (a.second == nullptr) {
			continue;
		}
		else {
			cout << a.first << " ";
			a.second->print2();
		}
		cout << endl;
	}
}


//还要添加，是否遍历到最后的判断
vector<string> Trie::find2(string str) {
	cout << "string Trie::find()" << endl;
	vector<string> ret;
	//先找第一个
	if (str.size() == 0) {
		cout << "Trie::find2(string str)没收到" << endl;
		return ret;
	}

	string temp;
	
	Trie* p = new Trie(*this);
	for (char c : str) {
		if (p->_childs[c - 'a'].second != nullptr) {
			p = p->_childs[c - 'a'].second;
			temp += c;
		}
		else {
			//不需要额外判断叶节点，叶节点所有second都是null，还是会进入else
			break;
		}
		
	}
	//如果字符串读取正确，就运行不到上面哪个else
	return ret;
}


vector<string> Trie::getPossible(string str) {
	cout << "string Trie::find()" << endl;
	vector<string> ret;
	//先找第一个
	if (str.size() == 0) {
		cout << "Trie::find2(string str)没收到" << endl;
		return ret;
	}

	string temp;

	Trie* p = new Trie(*this);
	Trie* pre = new Trie(*p);
	for (int i = 0;i<str.size();++i) {
		if (p->_childs[str[i] - 'a'].second != nullptr) {
			pre = p;
			p = p->_childs[str[i] - 'a'].second;
			temp.push_back(str[i]);
		}
		else {
			//此时一定是不完美匹配,
			//搜索的应该是前缀，而不是当前词，否则一旦前缀完美匹配，就不会搜索候选词
			ret = pre->search();
			string self = temp;
			self.push_back(ret[0][1]);
			temp.pop_back();
			for (auto& v : ret) {
				v = temp + v;
			}


			ret.push_back(self);
			return ret;
		}

	}
	//只输入前缀的情况
	if (p->_isEnd == false) {
		ret = pre->search();
		string self = temp;
		temp.pop_back();
		for (auto& v : ret) {
			v = temp + v;
		}
		ret.push_back(self);
		return ret;
	}
	//如果字符串读取正确，就运行不到上面哪个else
	ret.push_back(temp);
	return ret;
}


//实现字典树的广度遍历，同时输出所有可能的结果
//search不会返回前缀，只会返回起点开始的所有可能
vector<string> Trie::search() {
	//初始化系列
	vector<string> ret;
	deque<pair<string,Trie*>> tempBranch;
	pair<string, Trie*> tempNode=make_pair("",this);
	tempBranch.push_back(tempNode);
	//遍历系列
	while (tempBranch.size()!=0)
	{
		tempNode = tempBranch.front();
		tempBranch.pop_front();
		if (tempNode.second->_isEnd == true) {
			ret.push_back(tempNode.first);
			continue;
		}
		else
		{
			string tempStr;
			for (int i = 0; i< 26; ++i) {
				if (tempNode.second->_childs[i].second!=nullptr)
				{
 					char tempChar = 'a' + i;
					tempStr= tempNode.first;
					tempStr.push_back(tempChar);
					tempBranch.push_back(make_pair(tempStr,tempNode.second->_childs[i].second));
				}
			}
			
		}

		
	}
	_ret = ret;
	return ret;
}


void testSearch()
{
	vector<string> input;
	string s;
	while (cin >> s)
	{
		input.push_back(s);
	}


	Trie t;
	//T.print2();
	t.build2(input);
	input = t.search();
	t.debug();

	for (auto v : input) {
		cout << v << endl;
	}

}

void testPossible() {

	vector<string> input;
	string s;
	while (cin >> s)
	{
		input.push_back(s);
	}


	Trie t;
	//T.print2();
	t.build2(input);

	input = t.getPossible("byta");//输入前缀
	for (auto v : input) {
		cout << v << endl;
	}


	input = t.getPossible("bytedance");//输入同缀词
	for (auto v : input) {
		cout << v << endl;
	}
	
	input = t.getPossible("byte");//输入前缀
	for (auto v : input) {
		cout << v << endl;
	}




}

int main() {
	cout << "第n版" << endl;
	testPossible();
	system("pause");
	return 0;
}