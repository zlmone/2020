#include"../include/jsonPackage.h"
#include"../../../json包/jsoncpp-src-0.5.0/src/lib_json/json_value.cpp"
#include"../../../json包/jsoncpp-src-0.5.0/src/lib_json/json_writer.cpp"
#include"../../../json包/jsoncpp-src-0.5.0/src/lib_json/json_reader.cpp"
#include<string>
#include<sstream>
using namespace std;


namespace wd{

//默认传参不会超  过限制大小，所以在传参之前需要加判断
//同时传参进来的时候，必须按照优先级排序，因为封包的时候也排序了
string jsonPackage::packageJson(vector<string> vs){
    Json::FastWriter writerInfo;
    Json::Value data;

    data["num"]=to_string(vs.size());
    string temp;
    for(auto a:vs){
        temp+=a+" ";
    }
    data["words"]=temp;
    std::string str=writerInfo.write(data);
    return str;

}
string jsonPackage::packageArray(vector<string> vs){
    Json::FastWriter writerInfo;
    Json::Value dataArray;
    

    for(int i=0;i!=vs.size();++i){
        Json::Value Data;
        Data["len"]=to_string(vs[i].size());
        Data["data"]=vs[i];
        dataArray.append(Data);
    }
    
    return writerInfo.write(dataArray);
}




vector<string> jsonPackage::TranslateJson(const string strData){
    Json::Reader *readerInfo=new Json::Reader(Json::Features::strictMode());
    Json::Value root;
    cout<<"开始解析"<<endl;
    string s;
    if(readerInfo->parse(strData,root)){
        if ((root["num"].isInt()))
        {
            int len=root["num"].asInt();
        }
        if(root["words"].isString()){
            s=root["words"].asString();
        }
    }
    cout<<s<<endl;


    istringstream record(s);
    vector<string> vs;
    string word;
    while(record>>word){
        vs.push_back(word);
    }
    return vs;

}
vector<string> jsonPackage::TranslateArray(const string strData){
    Json::Reader *readerinfo =new Json::Reader(Json::Features::strictMode());
    cout<<endl<<"strData is"<<strData<<endl;

    Json::Value root;
    vector<string> vs;
    cout<<"开始解析"<<endl;
    readerinfo->parse(strData,root);
    //int nLen=root[0]["len"].asInt();
    //cout<<nLen;
    cout<<"解析完成"<<endl;
    if(readerinfo->parse(strData,root)){
        //解析所有成员
        cout<<"进if"<<endl;
        int nArraySize=root.size();
        cout<<nArraySize<<endl;
        if(root.isArray()){
            cout<<"又进if"<<endl;
            for(int i=0;i<nArraySize;++i){
                //cout<<"进for ";
                //cout<<i<<endl;//后面开始崩的
                cout<<root[i]["len"];
                cout<<root[i]["data"];
                int nLen=root[i]["len"].asInt();
                string word=root[i]["data"].asString();
                //string word=root[i]["data"];
                //cout<<word;
                //vs.push_back(word);
            }
        }    
    }
    cout<<"没进if"<<endl;
    ::delete readerinfo;
    readerinfo=NULL;
    return vs;
}




}



/*
int main(){
    jsonPackage js;
    vector<string> vs={"silly","dilly","jesse","bully"};
    for(auto a:vs){
        cout<<a<<endl;
    }
    string str=js.packageJson(vs);//封装完美


    vector<string> vs2=js.TranslateJson(str);
    cout<<"打印返回值"<<endl;
    for(auto a:vs2){
        cout<<a<<endl;
    }
    return 0;
}*/