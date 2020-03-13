#pragma once
#include<iostream>
#include<queue>
#include<vector>
#include<map>
using namespace std;


namespace wd
{
	

//使用类

//类模板的分离与实现，不得分开写
template<typename T>
class LRUwindow
{
public:
	LRUwindow(size_t s) :_maxSize(s) {}
	~LRUwindow() {}
	void  add(T n){
		cout<<"LRU add"<<endl;
		bool flag = false;
		for (auto it = _que.begin(); it != _que.end(); ++it) {
			if (*it == n) {
				flag = true;
				it = _que.erase(it);//֮�����뵽β��
				if (it == _que.end()) {
					break;
				}
			}
		}
	//1.û�е�ʱ��
	if (flag==false) {
		if (_que.size() >= _maxSize) {
			_que.erase(_que.begin());
		}
	}
	//2.�е�ʱ��
	else {
		
	}
	_que.push_back(n);
}



	void print(){cout<<"打印"<<endl;}
	size_t size(){return _maxSize;}
	T& operator[](int id){return _que[id];}

	vector<T> _que;
private:
	
	size_t _maxSize;
};







} // namespace wd


