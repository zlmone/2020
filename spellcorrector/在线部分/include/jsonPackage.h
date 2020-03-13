#include"json/json.h"
#include<vector>

using namespace std;
//#ifdef _DEBUG
//#pragma comment(lib,"./lib/json_vc71_libmtd.lib")


//单体封包完全解决，问题是数组封包解决不来


#include<vector>
using namespace std;


namespace wd{

class jsonPackage
{
public:
    jsonPackage(){;}
    ~jsonPackage(){;}

    //服务器传入构造函数，调用此函数打包打包
    string packageArray(vector<string> vs);
    string packageJson(vector<string> vs);//封装单个数据
    //解包
    vector<string> TranslateJson(const string strData);
    vector<string> TranslateArray(const string strData);
};

}


