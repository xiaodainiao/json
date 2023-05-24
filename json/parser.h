#pragma once
#include <string>
#include "json.h"
using namespace std;
namespace xiaodainiao{

namespace json{
class Parser
{
public:
    Parser();        //构造函数
    void load(const string& str); //需要解析的字符串

    Json parse();    //核心

private:
    void skip_write_space(); //将空白字符，回车，空行，换行,制表符等忽略掉
    char get_next_token();   //解析字符是从前往后，因此需要获取下一个字符的方法
    Json parse_null();
    Json parse_bool();
    Json parse_number();
    string parse_string();
    Json parse_array();
    Json parse_object();
private:
    string m_str;
    int m_idx;
};

}
}