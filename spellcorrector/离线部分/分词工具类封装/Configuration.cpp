/**
 * Project 基本网络编程工具
 */


#include "Configuration.h"
#include<string>
#include<map>
#include<utility>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;
/**
 * Configuration implementation
 */


/**
 * @param const string &filepath
 */


namespace wd{

    Configuration::Configuration(const string &filepath)
    :_filepath(filepath) {
        //cout << "Configuration::Configuration(const string &filepath)" << endl;
        //传一套默认参数进来，后续可以在文件中修改
        _configMap.insert(make_pair("ip_address","192.168.1.105"));
        _configMap.insert(make_pair("port","8111"));

        _configMap.insert(make_pair("ThreadNumber","4"));
        _configMap.insert(make_pair("TaskNum","10"));

        _configMap.insert(make_pair("cacheFile","缓存.txt"));
        _configMap.insert(make_pair("dictionary","字典.txt"));
        _configMap.insert(make_pair("cacheFile","索引.txt"));

        ifstream in(filepath);
        if (!in) {
            cout << "error file or not exist" << endl;
            return;
        }
        cout << "indexMachine::inputArticle(string path)" << endl;


        string Line;    
        while (getline(in, Line)) {             
                        //lines.push_back(Line);                //Ç°ÃæÈýÐÐÀàËÆÓÚ´òÓ¡½ø¶ÈÌõ£¬µ«ÊÇ´¦ÀíÌ«¿ì»á¿´²»µ½Õâ¸öÐ§¹û
            istringstream record(Line);     
           // cout << endl << "temp line is " << Line << endl;

            string index,content;
            record>>index;
            //cout<<"index is "<<index<<endl;;
            record>>content;
            //cout<<"content is "<<content<<endl;
            
            if(_configMap.find(index)!=_configMap.end()){
                _configMap[index]=content;
            }
            //_configMap.find(word)->second=content;
            //(_configMap.find(word))->second=content;
                        //使用这个方法会发生core dump，首先是有一定概率找不到的
                        //加了括号也会
            
        }
    }

/**
 * @return map<string string> &
 */
map<string,string>  &Configuration::getConfigMap() {
    return _configMap;
}

    void Configuration::printMap(){
        for(auto c:_configMap){
            cout<<c.first<<" "<<c.second<<endl;
        }
    }
}


void testConfiguration(){
    wd::Configuration conf("conf.txt");
    for(auto c:conf.getConfigMap()){
        cout<<c.first<<" "<<c.second<<endl;
    }

    //auto mp=conf.getConfigMap();
            //想要改变返回值为左值的函数，返回的值。不能给他另赋值对象
            //而是直接在函数上进行操作
    //cout<<mp.begin()->first<<endl;
    (conf.getConfigMap())["ip_address"]="1.1.1.1";
    cout<<"更改之后"<<endl;
    /*for(auto c:mp){
        cout<<c.first<<" "<<c.second<<endl;
    }*/
    cout<<endl;
    auto mp2=conf.getConfigMap();
    for(auto c:mp2){
        cout<<c.first<<" "<<c.second<<endl;
    }
}


int main()
{
    testConfiguration();
    wd::Configuration conf("conf.txt");
    map<string,string> mss=conf.getConfigMap();

    //以后所有的配置信息，都从mss中获取即可
    return  0;
}
