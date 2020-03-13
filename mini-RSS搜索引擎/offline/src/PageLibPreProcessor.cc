#include "../inc/PageLibPreProcessor.h"
#include"../inc/DirScanner.h"
#include"../inc/FileProcessor.h"
#include<fstream>
#include<sstream>
#include<math.h>
using namespace std;


namespace wd{



void PageLibPreProcessor::doProcess() {
    
    
    return;
}

void PageLibPreProcessor::readInfoFromFile() {
    //从conf文件中，读取已经写好的网页库，和偏移库，进行处理
    _confMap=_conf.getConfigMap();
    auto itPage=_confMap.find("网页库输出");
    if(itPage==_confMap.end()){
        cout<<"can not find 网页库"<<endl;
        return;
    }
    auto itOffset=_confMap.find("网页偏移库输出");
    if(itOffset==_confMap.end()){
        cout<<"can not find 网页偏移库"<<endl;
        return;
    }


    cout<<"开始读取"<<endl;
    ifstream inPage(itPage->second);
    ifstream inOffset(itOffset->second);
    //循环存入webPage，之后打入向量中
    string attribute,no,start,size;
    string str,line;//str 暂时保存当前网页已经读入的内容
                    //line保存一行，用于每轮并入str
    while (getline(inOffset,attribute))
    {
        istringstream record(attribute);
        record>>no>>start>>size;
        size_t len=stoi(size);
        pair<int,int> p=make_pair(stoi(start),len);
        _offsetLib.insert(make_pair(stoi(no),p));
        cout<<"读";

        getline(inPage,line);
        str+=line;
        if(str.size()==len){//正好是一个数据？？？？？？？？？？？？？？？？？？？？？？
            WebPage webPage(str,_conf,_jieba);
            _vecWebPage.push_back(webPage);
            str="";
        }
    }        
}
    

    



void PageLibPreProcessor::cutRedundantPaged() {
    for(auto i=_vecWebPage.begin();i!=_vecWebPage.end();++i){
        for(auto j=_vecWebPage.end()-1;j>=_vecWebPage.begin();--j){
            if(*j==*i){
                _vecWebPage.erase(j);
                int id=j->getDocId();
                _offsetLib.erase(id);
            }
        }
    }
    return;
}




void PageLibPreProcessor::buildInvertIndexTable() {
    int i=0;
    size_t j;
    double w[100][100];//第i个网页，第j个单词，在本网页中的权重
    double sumW;
    vector<string> tempWords;
    //对所有文档中的每一个主词,统计所有词出现的文档数
    map<string,int> totalFreq;
    for(i=0;i!=_vecWebPage.size();++i){
        tempWords=_vecWebPage[i].getWords();
        for(j=0;j!=tempWords.size();++j){
            if(totalFreq.find(tempWords[j])==totalFreq.end()){
                totalFreq.insert(make_pair(tempWords[j],1));
            }else{
                totalFreq[tempWords[j]]++;
            }
        }
    }

    
    //对所有文档中的每一个主词,计算权重
    for(i=0;i!=_vecWebPage.size();++i){
        //对文档中的每一个主词
        tempWords=_vecWebPage[i].getWords();
        for(j=0;j!=tempWords.size();++j){
            //计算词在所有文档中，出现的次数
            int df=totalFreq.find(tempWords[j])->second;      //整体词频
            //取出词在本文档中的词频
            map<string,int> msi=_vecWebPage[i].getWordsMap();//取出词频数据结构
            int tf=msi.find(tempWords[j])->second;           //从中找出词频
            //计算逆文档频率
            double n=(_vecWebPage.size()-1)/(df+1);
            double idf=log10(n)/log(2);
            //计算基本权重，并保存
            w[i][j]=tf*idf;
            _invertIndexTable[tempWords[j]][j]=make_pair(_vecWebPage[i].getDocId(),w[i][j]);
            sumW+=pow(w[i][j],2);
        }
        
    }
    
    



    //归一化权重
    for(i=0;i!=_vecWebPage.size();++i){
        //对文档中的每一个主词
        tempWords=_vecWebPage[i].getWords();
        for(j=0;j!=_vecWebPage[i].getWords().size();++j){
            double normW=w[i][j]/sqrt(sumW);
            //第一个索引为单词索引，第二个索引为文档编号索引；等号之后为其重新赋值
            _invertIndexTable[tempWords[j]][j]=make_pair(_vecWebPage[i].getDocId(),normW);
        }
        
    }
}


void PageLibPreProcessor::storeOnDisk() {
    cout<<"storeOnDisk()"<<endl;
    //保存去重之后的，网页库和网页偏移库
    auto itPage=_confMap.find("新网页库");
    if(itPage==_confMap.end()){
        cout<<"can not find 新网页库"<<endl;
        return;
    }
    auto itOffset=_confMap.find("新网页偏移库");
    if(itOffset==_confMap.end()){
        cout<<"can not find 新网页偏移库"<<endl;
        return;
    }
    auto itInvert=_confMap.find("倒排索引库");
    if(itInvert==_confMap.end()){
        cout<<"can not find 倒排索引库"<<endl;
        return;
    }

    ofstream outPage(itPage->second);
    ofstream outOffset(itOffset->second);
    ofstream outInvert(itInvert->second);
    //循环存入webPage，之后打入向量中
    string attribute,no,start,size;
    string str,line;//str 暂时保存当前网页已经读入的内容
                    //line保存一行，用于每轮并入str
    

    cout<<itPage->second<<" "<<itOffset->second<<" "<<itInvert->second<<endl;
    //保存新的网页库，偏移库
    for(auto v:_vecWebPage){
        outPage<<v.getDoc();
    }
    for(auto v:_offsetLib){
        outOffset<<v;
    }
    //保存倒排索引
    for(auto i:_invertIndexTable){
        outInvert<<i;
    }
    return;


}

}
