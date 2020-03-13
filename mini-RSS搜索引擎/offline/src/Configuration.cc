/**
 * Project 基本网络编程工具
 */


#include "../inc/Configuration.h"
#include<fstream>
#include<sstream>
using namespace std;

namespace wd{

Configuration::Configuration(const string& filepath) {
    ifstream in(filepath);
    string line;
    while (getline(in,line))
    {
        istringstream record(line);
        string item;
        string data;
        record>>item;
        record>>data;
        _configMap.insert(make_pair(item,data));
    }
    
}

}