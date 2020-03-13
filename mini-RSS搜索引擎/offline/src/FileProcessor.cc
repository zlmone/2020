/**
 * Project 基本网络编程工具
 */


#include "../inc/FileProcessor.h"
#include"../../tinyxml2/tinyxml2.h"
//#include"../../tinyxml2/tinyxml2.cpp"
#include"../inc/WebPage.h"
#include<cstdlib>
#include<sstream>
#include<regex>
using namespace std;


namespace wd{


//传入的xml文件内，一个文件会有多个内容，因此需要重写
//写成一个循环体才行
string FileProcessor::process(int &idx,const string &filePath){
    cout<<"string FileProcessor::process():filePath is"<<endl;
    cout<<filePath<<endl;
    /*
    string title,content,url;
    readfile(filePath,title,content,url);
    _titleFeature=title;*/
    
    string ret;
    tinyxml2::XMLDocument xmlDoc;
    xmlDoc.LoadFile(filePath.c_str());
    cout<<"filePath.c_str()"<< endl;
    //const char *xml=doc.c_str();
    //xmlDoc.Parse(xml);
    if(xmlDoc.ErrorID()){
        cout<<"XMLDocument load file error"<< endl;
        return "";
    }
    
    cout<<"开始解析2"<<" ";
    tinyxml2::XMLElement *xmlElem=xmlDoc.FirstChildElement("rss")
            ->FirstChildElement("channel")->FirstChildElement("item");
    cout<<"开始解析2"<<" ";//部分文件，会在这个地方，和下一个打印之间崩溃，神了。
    do{
        cout<<"标题解析"<<" ";
        tinyxml2::XMLElement *titleNode=xmlElem->FirstChildElement("title");
        const char *title=titleNode->GetText();
            //_docTitle=title;

        cout<<"url解析"<<" ";
        tinyxml2::XMLElement *UrlNode=xmlElem->FirstChildElement("link");
            const char *Url=UrlNode->GetText();
            //_docUrl=Url;

        cout<<"内容解析"<<" ";
        tinyxml2::XMLElement *contentNode=xmlElem->FirstChildElement("content:encoded");
        const char *content=contentNode->GetText();


        cout<<"_docTitle="<<title<<endl;
            //cout<<"_docUrl="<<_docUrl<<endl;
            //cout<<"_docContent="<<_docContent<<endl;


        cout<<"去除标记"<<endl;
        regex re("<.+?>");
        string contentProcessed=regex_replace(content,re,"--");
            //cout<<endl<<endl<<endl<<endl<<endl<<endl<<endl<<"newContent:"<<contentProcessed<<endl;
            //_docContent=contentProcessed;

        cout<<"开始构建字串"<<endl;
        ret+="<doc><docid>"+to_string(idx)+
                "</docid><url>"+Url+
                "</url><title>"+title+
                "</title><content>"+contentProcessed+
                "</content></doc>"+"\n\n\n";
        ++idx;
        } while (xmlElem=xmlElem->NextSiblingElement("item"));
        cout<<endl<<endl<<endl<<endl;
    return ret;
}

//使用webPage封装的工具，相当不严谨，可能要重写
void FileProcessor::readfile(const string &filename,string &title,string &content,string &urlAddr) {
    cout<<"FileProcessor::readfile() : "<<endl;
    cout<<filename<<endl;
        tinyxml2::XMLDocument xmlDoc;
        xmlDoc.LoadFile(filename.c_str());
        cout<<"_doc.c_str()"<<endl;
        //const char *xml=doc.c_str();
        //xmlDoc.Parse(xml);
        if(xmlDoc.ErrorID()){
            cout<<"XMLDocument load file error"<<endl;
            return;
        }
        /*
        _xmlElem=xmlDoc.FirstChildElement("rss")
            ->FirstChildElement("channel")->FirstChildElement("item");
        do{

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
*/
}
}


