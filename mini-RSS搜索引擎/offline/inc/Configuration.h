
#ifndef _CONFIGURATION_H
#define _CONFIGURATION_H
#include<string>
#include<vector>
#include<map>
#include<set>
#include<utility>
using namespace std;

namespace wd{

class Configuration {
private:
    string _filepath;
    map<string,string> _configMap;
    set<string> _stopWordList;
    vector<string> __sourceUml;

public:     
    Configuration(const string& filepath);
    map<string,string>& getConfigMap(){
        return _configMap;
    }
    set<string> getStopWordList(){
        return _stopWordList;
    }
};


}

#endif //_CONFIGURATION_H