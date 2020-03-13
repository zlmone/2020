#include"v2.h"
#include<deque>
#include<utility>
using namespace std;
//ԭ���汾��˼·�ǣ�ÿ�ν���һ���㣬Ȼ��ӵ׵���������
//���ܵĴ�������
//	�ס�ָ��Ľ����������ռ䣬��Ȼ��֪������ô��
				//Ҳ����˵������������ú�һ���ڵ��ж��ٸ�����
//		Ҫô���ܾ���Ҫ���ÿռ�Ķ�̬����
//		ʹ�öѽ��д洢������new������ռ䣩
//	�ҡ�˳�������Ҫ��ͷ����


void Trie::init() {
	_childs.resize(26);
	for (size_t i = 0; i < 26; ++i) {
		this->_childs[i].first = 'a' + i;
		this->_childs[i].second = nullptr;
	}
}


//�������֪����ô����ָ������ڴ棬д��������
void Trie::build2(vector<string> vs) {
	//ÿ������Ϊһ�֣����쵽��
	cout << "void Trie::build()����ǰ " << endl;
	for (auto s:vs) {
		Trie* temp=this;
		for (size_t i = 0;i!=s.size();++i) {
			Trie t;
			//��������
			if (s[i]>='a'&&s[i]<='z') {
				if (temp->_childs[s[i] - 'a'].second==nullptr) {
					temp->_childs[s[i] - 'a'].second =new Trie(t);
				}
				temp->_isEnd = false;
				temp = temp->_childs[s[i] - 'a'].second;
			}
			else
			{
				cout << "�ݲ�֧�ַ�Ӣ����" << endl;
				goto end;
			}

		}
		temp->_isEnd = true;
	end:
		cout << "��һ��" << endl;
	}
	


}


/*
void Trie::build(vector<vector<string>> vvs) {
	//ÿ������Ϊһ�֣����쵽��
	cout << "void Trie::build() " << endl;
	for (auto vs : vvs) {
		Trie child;
		child._childs.insert(make_pair(vs[vs.size() - 1], nullptr));
		Trie* son = &child;
		//�����ǿ�ָ�룬����Ҫ������嶫��


	   //ÿ���֣�����һ���ڵ�

		for (size_t s = vs.size() - 2; s >= 0; --s) {
			Trie temp;
			//��������
			if (s == 0) {
				_childs.insert(make_pair(vs[s], son));

				break;
			}
			else
			{
				//cout << "�ж�son" << endl;
				if (son == nullptr) {
					cout << "son�Ǹ���ָ��" << endl;
				}
				else {
					for (auto a : son->_childs) {
						cout << a.first << " " << a.second << endl;
					}
				}
				//�����ʱ�������,����ȥ�������ָ���в�������
				(temp._childs).insert(make_pair(vs[s], son));
				Trie* tempSon = &temp;
				son = tempSon;		//д��son=&tempҲ��֪���Բ���
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


//��Ҫ��ӣ��Ƿ�����������ж�
vector<string> Trie::find2(string str) {
	cout << "string Trie::find()" << endl;
	vector<string> ret;
	//���ҵ�һ��
	if (str.size() == 0) {
		cout << "Trie::find2(string str)û�յ�" << endl;
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
			//����Ҫ�����ж�Ҷ�ڵ㣬Ҷ�ڵ�����second����null�����ǻ����else
			break;
		}
		
	}
	//����ַ�����ȡ��ȷ�������в��������ĸ�else
	return ret;
}


vector<string> Trie::getPossible(string str) {
	cout << "string Trie::find()" << endl;
	vector<string> ret;
	//���ҵ�һ��
	if (str.size() == 0) {
		cout << "Trie::find2(string str)û�յ�" << endl;
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
			//��ʱһ���ǲ�����ƥ��,
			//������Ӧ����ǰ׺�������ǵ�ǰ�ʣ�����һ��ǰ׺����ƥ�䣬�Ͳ���������ѡ��
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
	//ֻ����ǰ׺�����
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
	//����ַ�����ȡ��ȷ�������в��������ĸ�else
	ret.push_back(temp);
	return ret;
}


//ʵ���ֵ����Ĺ�ȱ�����ͬʱ������п��ܵĽ��
//search���᷵��ǰ׺��ֻ�᷵����㿪ʼ�����п���
vector<string> Trie::search() {
	//��ʼ��ϵ��
	vector<string> ret;
	deque<pair<string,Trie*>> tempBranch;
	pair<string, Trie*> tempNode=make_pair("",this);
	tempBranch.push_back(tempNode);
	//����ϵ��
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

	input = t.getPossible("byta");//����ǰ׺
	for (auto v : input) {
		cout << v << endl;
	}


	input = t.getPossible("bytedance");//����ͬ׺��
	for (auto v : input) {
		cout << v << endl;
	}
	
	input = t.getPossible("byte");//����ǰ׺
	for (auto v : input) {
		cout << v << endl;
	}




}

int main() {
	cout << "��n��" << endl;
	testPossible();
	system("pause");
	return 0;
}