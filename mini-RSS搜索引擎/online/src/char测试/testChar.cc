#include<iostream>
using namespace std;



int main(){
	char *c=new char(12);
	for(int a=0;a!=12;++a){
		cout<<c[a]<<" ";
		int i=c[a];
		cout<<i<<endl;
	}
	return 0;
}
