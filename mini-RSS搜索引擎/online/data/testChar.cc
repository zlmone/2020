#include<iostream>
using namespace std;

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>


int main(){
	char buf[12];
	char buf2[12]={0};
	char *buf3=new char(12*sizeof(char));

	cout<<buf<<endl
		<<buf2<<endl
		<<buf3<<endl;

	int fd=open("测试.txt",O_RDONLY);
	read(fd,buf2,sizeof(buf2))





}
