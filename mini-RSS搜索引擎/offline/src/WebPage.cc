#include"../inc/WebPage.h"
#include"../../tinyxml2/tinyxml2.h"
#include"../../tinyxml2/tinyxml2.cpp"
#include<iostream>
#include<regex>
#include<queue>
#include<set>
#include"../inc/KthLargestString.h"
#include"../inc/MyDict.h"
#include"../inc/Configuration.h"
using namespace std;
using namespace tinyxml2;

namespace wd{
    MyDict *MyDict::_pInstance=nullptr;
    const int  WebPage::TOPK_NUMBER=20;


    WebPage:: WebPage(string &doc,Configuration &config,SplitToolCppJieba &jieba)
    :_doc(doc)
    //,_xmlDoc(new XMLDocument())
    {
        //配置文件传进来干嘛
        //分词文件暂未用到
        //下面对传进来的css代码进行解析
        cout<<"webPage:string: "<<_doc<<endl;
        cout<<"_doc.c_str()"<<endl;
        const char *xml=doc.c_str();
        tinyxml2::XMLDocument *xmlDoc;
        xmlDoc->Parse(xml);
        if(xmlDoc->ErrorID()){
            cout<<"XMLDocument load file error"<<endl;
            return;
        }



        _xmlElem=xmlDoc->FirstChildElement("rss")
            ->FirstChildElement("channel")->FirstChildElement("item");
        do{
            tinyxml2::XMLElement *idNode=_xmlElem->FirstChildElement("docid");
            const char *id=idNode->GetText();
            _docId=stoi(id);
            
            tinyxml2::XMLElement *titleNode=_xmlElem->FirstChildElement("title");
            const char *title=titleNode->GetText();
            _docTitle=title;


            tinyxml2::XMLElement *UrlNode=_xmlElem->FirstChildElement("link");
            const char *Url=UrlNode->GetText();
            _docUrl=Url;


            tinyxml2::XMLElement *contentNode=_xmlElem->FirstChildElement("content:encoded");
            const char *content=contentNode->GetText();


            cout<<"_docTitle="<<_docTitle<<endl;
            cout<<"_docUrl="<<_docUrl<<endl;
            //cout<<"_docContent="<<_docContent<<endl;


            std::regex re("<.+?>");
            string contentProcessed=std::regex_replace(content,re,"--");
            //cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<"newContent:"<<contentProcessed<<endl;
            _docContent=contentProcessed;

        } while (_xmlElem=_xmlElem->NextSiblingElement("item"));

    }    







/*造轮子版proceDoc
void WebPage::processDoc(/*const string &doc,Configuration &config,WordSegmentation&){
    
    
    MyDict *p=MyDict::createInstance();
    p->init(_docContent);

    KthLargestString wordsMap(8,p->getDict(),p->getIndexTable());
    _topWords=wordsMap.retHeap();
    _wordsMap=wordsMap.retFrequency();
}
*/
void WebPage::processDoc(const string &doc,Configuration &config,SplitToolCppJieba& jieba){
    //分词
    vector<string> Vec=jieba.cut(_docContent);
    auto confMap=config.getConfigMap();
    auto it=confMap.find("停用词集");
    if(it==confMap.end()){
        cout<<"can not find 停用词集"<<endl;
        return;
    }
    //读取停用词集
    ifstream in(it->second);
    string line;
    set<string> ss;
    while (getline(in,line))
    {
        istringstream record(line);
        string word;
        while (record>>word)
        {
            ss.insert(word);
        }
        
    }
    calcTopK(Vec,TOPK_NUMBER,ss);
}

struct node{
    pair<string,int> freq;
    bool operator<(node x) const{
        return this->freq.second<x.freq.second;
    }
    node(pair<string,int> p):freq(p){}
};

//选择前k个，这个用另外两个类搞完了
void WebPage::calcTopK(vector<string> &wordsVec,int k,set<string> &stopWordList){
    //去掉停用词
    cout<<"void WebPage::calcTopK()"<<endl;    
    for(string c:wordsVec){
        if(stopWordList.find(c)!=stopWordList.end()){
            continue;
        }
        if(_wordsMap.find(c)!=_wordsMap.end()){
            ++_wordsMap[c];
        }else{
            _wordsMap.insert(make_pair(c,1));
        }
    }
    //优先队列用于选择前K个词
    priority_queue<node> pq;
    for(auto m:_wordsMap){
        pq.push(node(m));
    }
    for(int i=0;i!=TOPK_NUMBER;++i){
        node n=pq.top();
        pq.pop();
        _topWords.push_back(n.freq.first);
    }
}



//大概是比较两个xml文档
//top10词交集大于6个，则认为是重复                              还没测试
bool operator==(const WebPage &lhs,const WebPage &rhs){
  int count=6;
  for(size_t i=0;i<10;++i){
      for(auto a:rhs._topWords){
        if(lhs._topWords[i]==a){
          --count;
        }
      }
  }
  if(count<=0){
      return true;
  }else{
      return false;
    }
}
//
bool operator<(const WebPage &lhs,const WebPage &rhs){
    ;
}

}


/*
int test(){
    string path="coolshell.xml";
    wd::WebPage webPage(path);
    //webPage.getDoc();
    webPage.processDoc();
    map<string,int> msi=webPage.getWordsMap();
    for(auto m:msi){
        cout<<m.first<<" "<<m.second<<endl;
    }

}*/

/*
int main(){
    test();
    return 0;
}*/