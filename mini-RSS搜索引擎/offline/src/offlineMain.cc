#include"../inc/Configuration.h"
#include"../inc/DirScanner.h"
#include"../inc/FileProcessor.h"
#include"../inc/PageLib.h"
#include"../inc/WebPage.h"
#include"../inc/PageLibPreProcessor.h"
#include<iostream>
using namespace std;


int main(){
    wd::Configuration conf("../conf/conf.txt");
    wd::FileProcessor fileProcessor(conf);
    wd::DirScanner dirScanner(conf);
    wd::PageLib pageLib(conf,dirScanner,fileProcessor);
    cout<<endl<<endl<<endl<<"创建并保存网页库，网页偏移库"<<endl;
    pageLib.create(); 
    pageLib.store();


    cout<<endl<<endl<<"预处理第二阶段"<<endl;
    wd::PageLibPreProcessor pagelibPre(conf);
    pagelibPre.readInfoFromFile();
    pagelibPre.cutRedundantPaged();
    pagelibPre.buildInvertIndexTable();
    pagelibPre.storeOnDisk();
    //pagelibPre.doProcess();
    return 0;
}