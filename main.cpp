#include <iostream>
#include "json.h"
using namespace xiaodainiao::json;
using namespace std;
#include <fstream>
#include <sstream>

int main()
{
    //===================基本类型转换为Json类型============================
    Json v1;
    Json v2 = true;
    Json v3 = 123;
    Json v4 = 1.23;
    Json v5 = "hello, world";


    //json转换为基本类型  bool b = v2;这就需要实现基本类型的运算符重载
     bool b = v2;
     int i = v3;
     double f = v4;
     const string &s = v5;

    cout << "==========================数组与json类型的转换======================" << endl;
    //======================数组与json类型的转换===========================
    Json arr;
    arr[0] = true;
    arr[1] = 123;
    arr.append(1.23);
    arr.append("hello world");

    std::cout << arr.str() << std::endl;

    bool b1 = arr[0];
    int i1 = arr[1];
    double f1 = arr[2];
    const string& s1 = arr[3];


    cout << "==========================结构体与json类型的转换======================" << endl;
    //========================结构体与json类型的转换============================
    Json obj;
    obj["bool"] = true;
    obj["int"] = 123;
    obj["double"] = 1.23;
    obj["str"] = "hello world";
    std::cout << obj.str() << std::endl;


    cout << "==========================遍历数组内容======================" << endl;
    //=============================遍历数组内容==========================
    Json arr1;
    arr1[0] = true;
    arr1[1] = 123;
    arr1[2] = 1.23;
    arr1[3] = "hello world";
    for (auto it = arr.begin(); it != arr.end(); it ++){
        std::cout << (*it).str() << std::endl;
    }


    cout << "==========================判断数组或对象元素是否存在======================" << endl;
    //=========================判断数组或对象元素是否存在=====================
    std::cout << arr1.has(1) << std::endl;
    std::cout << arr1.has(6) << std::endl;

    
   cout << "==========================删除某个元素，在返回结果======================" << endl;
    arr1.remove(0);
    cout << arr1.str() << endl;

    cout << obj.has("hahah") << endl;
    cout << obj.has("str") << endl;

    obj.remove("str");
    cout << obj.str() << endl;


    cout <<"=========================解析===================================="<< endl;
    //const string & str = "[\"a\", 1, 2, 3, true, false, null, 123]";
    //const string & str = "{\"a\": 1, \"b\": 2, \"c\": 3}";
    //const string & str = "{"code":0,"message":"0","ttl":1,"data":{"refresh":false,"timestamp":1684815765863}}"
    
    ifstream fin("./test.json");
    stringstream ss;
    ss << fin.rdbuf();

    const string & str = ss.str();
    // cout << str << endl;
    Json v;
    v.parse(str);   //将字符串解析为JSON数据并存储在v中
    cout << v.str() << endl;

    cout << "JSON解析器" << std::endl;
    return 0;
}