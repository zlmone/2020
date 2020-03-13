#include<iostream>
using namespace std;


//离线部分主函数
//预定的编码顺序

//最高优先级：  学一下怎么处理网页文件 
//高优先级 ：   WebPage，FileProcessor
//中优先级 ：   PageLib，WordSegment（可能不用写）
//低优先级 ：   PageLibPrepropocessing，
//最低优先级：  configuration
//暂时先不写：  DirScanner
/*
int main1(){
    //configuration类
            //配置文件类要根据后续需求更改
            //因此在离线部分即将完结的时候再写
    cout<<"配置文件传入，构造"<<endl;

    //PageLib类                             DirScanner主要用于搜索语料库，并非是主要类，留后面再写
            //                              FileProcessor,对单篇文档进行处理，可能需要先写，PageLib需要用到它的功能
            //网页库
            //网页偏移库
    cout<<"配置文件传入PageLib类，并被修改"<<endl;


    //PageLibPrepropocessing类              主要被操作对象是WebPage,用到的工具是WordSegment,当然也可能是使用别的办法
            //网页去重
            //倒排索引
            //重建网页库
    cout<<"配置文件传入PageLibPrepropocessing，并被修改"<<endl;

    
}


//在线部分主函数
int main2(){
    //配置文件
    cout<<"配置文件传入，读取信息"<<endl;
    //线程池
    cout<<"配置文件传入，线程池信息"<<endl;
    //网络框架
    cout<<"配置文件传入，网络信息"<<endl;

    //注册函数三个
    //onMessage添加业务逻辑
            //查询
            //封包发送
    cout<<"onConnection"<<endl;
    cout<<"onConnection"<<endl;
    cout<<"onConnection"<<endl;
}


int main(){
    //建成之后，永久只需执行一次，除非重新配置服务器文件
    main1();
    
    //每次打开服务器自己运行即可
    main2();
}*/