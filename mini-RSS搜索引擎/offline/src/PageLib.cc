#include "../inc/PageLib.h"
#include<fstream>
#include<sstream>
#include<iostream>
using namespace std;

namespace wd{

PageLib::PageLib(Configuration &conf,DirScanner &dirScanner,FileProcessor &fileProcessor)
:_conf(conf)
,_dirScanner(dirScanner)
,_fileProcessor(fileProcessor)
{
    cout<<"PageLib::PageLib()"<<endl;
    map<string,string> mss=_conf.getConfigMap();
    auto outPage=mss.find("语料库文件目录");
    _dir=outPage->second;
    _dirScanner(_dir);
}

void PageLib::create() {
    cout<<"void PageLib::create() "<<endl;
    //取库文件的所有文件名，要在dirScanner类里面保证必须合法
    vector<string> temp=_dirScanner.files();
    int i=1;
    //只要路径必定合法，for循环就可以保证编号必定完全连续
    for(auto s:temp){
        string title,content,url;
        string str=_fileProcessor.process(i,_dir+"/"+s);//预处理算法没发现不合法的，可能性
        cout<<"i="<<endl;
        _vecFilesfiles.push_back(str);
    }

}

void PageLib::store() {
    cout<<"void PageLib::store() "<<endl;
    map<string,string> mss=_conf.getConfigMap();
    auto outRipePage=mss.find("网页库输出");
    auto outOffsetPage=mss.find("网页偏移库输出");
    string path1=outRipePage->second;
    string path2=outOffsetPage->second;
    cout<<path1<<"和"<<path2<<endl;
    ofstream out(path1);
    ofstream outSize(path2);

    cout<<"开始读取"<<endl;
    size_t formerLoc=0;
    size_t endLoc=0;
    int i=1;
    //create创建之初，就已经确保编号完全连续
    //那么你自己设置的编号一定和create里面的是对应的
    //但是如果有更好的办法，最好还是自己读一下编号，代码才会鲁棒。。。。。。。。。。。。。。。。。。重构预警
    for(auto v:_vecFilesfiles){
        out<<v<<endl<<endl<<endl;
        endLoc=formerLoc+v.size();
        outSize<<i<<" "<<formerLoc<<" "<<endLoc<<"换行"<<endl;
        formerLoc=endLoc;
        ++i;
        cout<<"写";
    }
}

}

void testPageLib(){
    wd::Configuration conf("../conf/conf.txt");

    wd::FileProcessor fileProcessor(conf);
    wd::DirScanner dirScanner(conf);
    wd::PageLib pageLib(conf,dirScanner,fileProcessor);

    cout<<endl<<endl<<endl<<"创建并保存文件"<<endl;
    pageLib.create();
    pageLib.store();
}
/*
int main(){
    testPageLib();
}*/
