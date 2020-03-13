/**
 * Project 基本网络编程工具
 */


#include "DictProducer.h"
#include<fstream>
#include<sstream>
#include<iostream>
#include"SplitToolCppJieba.h"
using namespace std;

namespace wd{

DictProducer::DictProducer(const string &dir) {
    
}


DictProducer::DictProducer(const string &dir,SplitToolCppJieba *splitTool)
//:_splitTool(new SplitToolCppJieba*(splitTool))
:_splitTool(splitTool)
{
    fstream fileIn(dir);
    string line;
    vector<string> tempVector;
    while(getline(fileIn,line)){
        tempVector=_splitTool->cut(line);
        _chineseWareHouse.insert(_chineseWareHouse.end(),_chineseWareHouse.begin(),_chineseWareHouse.end());
    }
}


void DictProducer::bulid_cn_dict(const string &dir) {
    ofstream fileOut(dir);
    for(auto w:_chineseWareHouse){
        fileOut<<w<<endl;
    }
    return;
}


void DictProducer::build_dict() {
    return;
}



void DictProducer::store_dict(const string &filename) {
    return;
}

/**
 * @return void
 */
void DictProducer::show_dict() {
    return;
}

/**
 * @return void
 */
void DictProducer::show_files() {
    return;
}

/**
 * @return void
 */
void DictProducer::get_files() {
    return;
}

/**
 * @param const string &word
 * @return void
 */
void DictProducer::push_dict(const string &word) {
    return;
}

DictProducer::~DictProducer() {

}

}
