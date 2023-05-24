#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;
namespace xiaodainiao{   //自定义命名空间，进行隔离
namespace json{

class Json
{

public:
    enum Type
    {
        json_null = 0,
        json_bool,
        json_int,
        json_double,
        json_string,
        json_array,
        json_object
    };

    Json();
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char* value);
    Json(const string& value);
    Json(Type type);
    Json(const Json& other);

    operator bool();
    operator int();
    operator double();
    operator string();

    //数组重载
    Json & operator []  (int index);
    void append(const Json& other);

    //结构体重载
    Json & operator []  (const char * key);
    Json & operator []  (const string& key);

    //需要添加一个赋值的重载运算符
    void operator = (const Json& other);

    void copy(const Json& other);

    void clear();  //对New出来的进行回收，防止内存泄露

    string str()  const;   //主要为了看到arr中存储的是什么值

    //定义数组的迭代器
    typedef std::vector<Json>::iterator iterator;
    iterator begin()
    {
        return m_value.m_array->begin();   
    }
    iterator end()
    {
        return m_value.m_array->end();
    }

    bool operator == (const Json& other);
    bool operator != (const Json& other);

    bool isNull()const{return m_type == json_null;}
    bool isBool()const{return m_type == json_bool;}
    bool isInt()const{return m_type == json_int;}
    bool isDouble()const{return m_type == json_double;}
    bool isString()const{return m_type == json_string;}
    bool isArray()const{return m_type == json_array;}
    bool isObject()const{return m_type == json_object;}

//显示转换
    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    string asString() const;

//判断数组中有没有某个索引，对象中有没有某个key
bool has(int index);
bool has(const char * key);
bool has(const string& key);

//移除元素
void remove(int index);
void remove(const char* key);
void remove(const string & key);

void parse(const string & str);

private:
    //联合体的内存字段是公用的，因此union的内存大小是由下面这些占用内存最大的那个
    union Value
    {
        bool m_bool;
        int m_int;
        double m_double;
        string *m_string;
        vector<Json> *m_array;
        map<string, Json> *m_object;

    };
    Type m_type;   //JSON包含类型和值
    Value m_value;
};


}}