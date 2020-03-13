#include"../include/WordQuery.h"
#include"../include/GlobalDefine.h"
#include"../../../json包/jsoncpp-src-0.5.0/src/lib_json/json_value.cpp"
#include"../../../json包/jsoncpp-src-0.5.0/src/lib_json/json_writer.cpp"
#include"../../../json包/jsoncpp-src-0.5.0/src/lib_json/json_reader.cpp"
//#include"../../../json包/jsoncpp-src-0.5.0/src/lib_json/
#include<sstream>
#include<iostream>
#include<json/json.h>
using namespace std;

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<cmath>
namespace wd{

 //自己写的
string WordQuery::doQuery(const string &str) {
    cout<<"string WordQuery::doQuery(const string &str)"<<endl;
    cout<<"传参"<<str<<endl;
    //做个分词，去除冗余词,要用到分词工具
    vector<string> words;
	words = _wordSeg(str.c_str());
	_wordSeg.getRidOfStopwords(words);
    
    for(auto s:words){
        cout<<s<<" ";
    }
    cout<<endl;
    //计算权重，进行排序
    vector<double> weight=getQueryWordsWeightVector(words);
	vector<pair<int, vector<double>>> vp;
    

        cout<<"doQuery测试取值"<<endl;
    if(excuteQuery(words,vp)==true){
		vector<int> ret=cosSimilar(vp,weight);
        string temp=createJson(ret,words);
        cout<<"doQuery查询结果大小"<<temp.size()<<endl;
        return temp;
    }else{
        cout<<"没找到";
        return returnNoAnswer();
    }
}


//大概是写完了
//是看了一点答案的
void WordQuery::loadLibrary() {
    cout<<"void loadLibrary() {"<<endl;
    //WordSegmentation jieba;
    map<string,string> mss=_conf.getConfigMap();
    string pagelib=mss[NEWPAGELIB_KEY];
    string offset=mss[NEWOFFSETLIB_KEY];
    string index=mss[INVERTINDEX_KEY];
    ifstream inPage(pagelib);
    ifstream inOffset(offset);
    ifstream inIndex(index);



    cout<<"读取索引"<<endl;
    string indexLine;
    while (getline(inIndex,indexLine))
    {
        istringstream stringIndex(indexLine);
        string word;
        int filename;
        double weight;
        stringIndex>>word;
        map<int,double> sp;
        while (stringIndex>>filename>>weight)
        {
            sp.insert(make_pair(filename,weight));
        }
        _invertIndexTable.insert(make_pair(word,sp));

    }


    
    cout<<"读网页库"<<pagelib.c_str()<<endl;
    int fd=open(pagelib.c_str(),O_RDONLY);
    cout<<"打开成功"<<endl;
    if(-1==fd){
        perror("open file fail");
    }
    string offsetLine,tempRead;
    cout<<"循环"<<endl;
    while (getline(inOffset,offsetLine))
    {
        istringstream stringOff(offsetLine);
        int no,start,size;
        stringOff>>no>>start>>size;
        _offsetLib.insert(make_pair(no,make_pair(start,size)));

        cout<<no<<" "<<start<<" "<<size;
        lseek(fd,start,SEEK_SET);           //偏移游标
        //cout<<"偏移成功"<<endl;
        //char *buf=new char(512);
        char buf[512]={0};                                                          //吐核的位置是最后一次打印之后的所有位置
                                                                                    //不见得是打印刚刚结束的位置
        //char buf[512];                         
        //char *buf=new char(128*sizeof(char*));//malloc():memary corruption


        //需要循环读，一个网页就会有上万偏移
        //cout<<"读取";
        string doc;
        size_t tempSize=512;
        if(tempSize>=size){
            int ret=read(fd,buf,sizeof(buf));//每次读取肯定要sizeof(buf)    
            if(ret==-1){
                perror("read error");
            }
            string temp=buf;
            doc+=temp;
            cout<<"短文档"<<endl;
        }
        while (tempSize<size)
        {
            int ret=read(fd,buf,sizeof(buf));//每次读取肯定要sizeof(buf)    
            if(ret==-1){
                perror("read error");
            }
            string temp=buf;
            doc+=temp;
            if(tempSize==size){
                break;
            }
            tempSize-=ret;
            cout<<".";
        }
        
        
                                                                        //在第40个网页的地方崩溃，建议单独打印第40个网页
                                                                        //和第41个网页
        WebPage *web=new WebPage(doc,_conf,_wordSeg);
        cout<<no<<  "存容器，当前尺寸"<<_pagelib.size()<<endl;
        _pagelib.insert(make_pair(no,*web));
        free(web);
        web=nullptr;
        cout<<"本轮结束"<<endl;
    }
}


//计算各个词的权重，返回一个权重集合，对应各个词的权重
//大概也是写完了
vector<double> WordQuery::getQueryWordsWeightVector(vector<string> &queryWords){
    cout<<"vector<double> WordQuery::getQueryWordsWeightVector(vector<string> &queryWords)"<<endl;
    vector<double> ret;
    double sum=0;
    map<string,double> msi;//权重得是int
    cout<<"统计词频"<<endl;
    for(auto s:queryWords){
        if(msi.find(s)!=msi.end()){
            ++msi[s];
        }else{
            msi.insert(make_pair(s,1));
        }
        cout<<s<<" ";
    }
    cout<<endl;

    cout<<"计算权重"<<endl;
    for(auto s:msi){
        //计算TF,在文章中出现的次数
        double tf=s.second;
        //计算DF,
        double df=1;
        //计算IDF，以及权重
        double w=tf*log(1/(df+1))/log(2);
        //归一化权重，存入容器
        sum+=pow(w,2);
        ret.push_back(w);
        cout<<tf<<" "<<df<<" "<<w*w<<endl;
    }

    cout<<"标准化 ";
    for(auto &d:ret){
        cout<<d<<" ";
        d=d/sqrt(sum);
        cout<<d<<endl;
    }
    cout<<endl;
    _search=ret;
    return ret;
}


//传入搜索词检索列表，第二参应该是传出参数，查找比对成功则返回true
//大概也是写完了
bool WordQuery::excuteQuery(const vector<string> &queryWords,
                        vector<pair<int,vector<double>>> &resultVec){
   
   
   //看了框架之后重写的方法
   cout<<"bool WordQuery::excuteQuery"<<endl;
   if(_invertIndexTable.find(queryWords[0])==_invertIndexTable.end()){
       return false;
   }else{
       //建立框架，也就是第一列
       for(auto doc:_invertIndexTable[queryWords[0]]){
           vector<double> temp;
           temp.push_back(doc.second);
           resultVec.push_back(make_pair(doc.first,temp));
           cout<<doc.first<<" ";
       }
   }
   
   cout<<"读取倒排索引"<<endl;
   auto it=queryWords.begin();
   for(++it;it!=queryWords.end();++it){
       cout<<*it<<endl;
       auto t=it;
       ++t;
       if(t==queryWords.end())break;//发送来的数据，最后面会多个空词
       if(_invertIndexTable.find(*it)==_invertIndexTable.end()){
           cout<<"???";
           return false;
       }else{
           //以你实现建立好的框架为基础，查找新的关键词，的文档列表中
           //是否有之前已经建立好的那些文档，没有就删除整行
           
			//_invertIndexTable[*it]指的是返回算法的每一行，的文档-权重映射
		   //it2指向各行行号，即文档编号
		   for (auto it2 = resultVec.begin(); it2 != resultVec.end();) {
			   if (_invertIndexTable[*it].find(it2->first) == _invertIndexTable[*it].end()) {
				   it2=resultVec.erase(it2);
			   }
			   else {
				   it2->second.push_back(_invertIndexTable[*it][it2->first]);
                   cout<<_invertIndexTable[*it][it2->first]<<endl;
				   ++it2;
			   }
           }
           cout<<"一轮";
       }
       cout<<"外层";
   }
   cout<<"读取完毕"<<endl;
   return true;
}











//给余弦相似度算法使用
static bool cmp(pair<int,double> p1,pair<int,double> p2){
    return p1.second<p2.second;
}
//暂时全部使用最笨方法，之后再重构
//算是写完了
vector<int> WordQuery::cosSimilar(vector<pair<int, vector<double>>>& input, vector<double> Query) {
    cout<<"vector<int> WordQuery::cosSimilar"<<endl;
    for(auto p:input){
        cout<<p.first<<":";
        for(auto d:p.second){
            cout<<d<<" ";
        }
        cout<<endl;
    }
    //求模
    vector<pair<int,double>> ret;

	int size = input[0].second.size();
    for(auto p:input){
		double sum=0.0;
		double vecMod = 0.0;
		for (unsigned int i = 0; i < size;++i) {
            sum+=p.second[i]*Query[i];          //累计向量积    
			vecMod += p.second[i] * p.second[i];//累计向量模
        }
		ret.push_back(make_pair(p.first, sum/sqrt(vecMod)));
        cout<<sum/sqrt(vecMod)<<" ";
    }
    
    
    
    sort(ret.begin(),ret.end(),cmp);            //没办法排序，要改
    vector<int> vecId;
    for(auto p:ret){
        vecId.push_back(p.first);
    }
    return vecId;
}







//要调用网页类
string WordQuery::createJson(vector<int> &docIdVec,const vector<string> &queryWords){
    cout<<"string WordQuery::createJson"<<endl;
    string str="";
    for (auto i : docIdVec) {
        cout<<i<<" ";
    }
    /*cout<<"开始封包"<<endl;
    _pagelib[250].primeSummary();
    _pagelib[250].getTitle();
    cout<<"测试完毕"<<" ";
    _pagelib[363].getTitle();*/
    for (auto i : docIdVec) {
        cout<<i<<" ";
		Json::FastWriter writerInfo;
		Json::Value data;
		data["num"] = to_string(i);
		data["title"] = _pagelib[i].getTitle();
		//提取摘要
		data["sumary"] = _pagelib[i].primeSummary();
        string temp=writerInfo.write(data);
        //cout<<temp<<endl;
		str +=  temp+ "\n";//这里应该是没问题
	}
    cout<<"createJson返回值大小"<<str.size()<<endl;
	return str;
}


string WordQuery::returnNoAnswer() {
        return "no answer";
}

/*WordQuery::WordQuery() {

}*/

void WordQuery::dataPrint(){
    cout<<"打印索引"<<endl<<endl;
    for (auto p:_invertIndexTable)
    {
        cout<<p.first<<":";
        for(auto a:p.second){
            cout<<a.first<<"-"<<a.second<<" ";
        }
        cout<<endl;
    }
    
   cout<<"打印偏移"<<endl<<endl;
    for (auto p:_offsetLib)
    {
        cout<<p.first<<":"<<p.second.first<<" "<<p.second.second<<endl;
    }
    
}


}




