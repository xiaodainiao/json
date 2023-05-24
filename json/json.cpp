#include "json.h"
#include <sstream>
#include "parser.h"
using namespace std;

using namespace xiaodainiao::json;

//都是先把类型初始化好m_type(json_null)，然后初始化value
Json::Json() : m_type(json_null)
{

}

Json::Json(bool value) : m_type(json_bool)
{
    m_value.m_bool = value;
}
Json::Json(int value) : m_type(json_int)
{
    m_value.m_int = value;
}

Json::Json(double value) : m_type(json_double)
{
   m_value.m_double = value;
}

Json::Json(const char* value) : m_type(json_string)
{
    m_value.m_string = new string(value);
}

Json::Json(const string& value) : m_type(json_string)
{
    m_value.m_string = new string(value);
}
Json::Json(Type type) : m_type(json_array)
{
    switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = false;
            break;
        case json_int:
            m_value.m_int = 0;
            break;
        case json_double:
            m_value.m_double = 0.0;
            break;
        case json_string:
            m_value.m_string = new string("");
            break;
        case json_array:
            m_value.m_array = new std::vector<Json>();
            break;
        case json_object:
            m_value.m_object = new std::map<string, Json>();
            break;
        default:
            break;
    }
}
Json::Json(const Json& other) : m_type(json_object)
{
    copy(other);
}

Json::operator bool()
{
    //如果不是对象，就转换为对象
    if (m_type != json_bool)
    {
        throw new logic_error("type error, not bool value");
    }
    return m_value.m_bool;
}
Json::operator int()
{
    if (m_type != json_int)
    {
        throw new logic_error("type error, not int value");
    }
    return m_value.m_int;
}
Json::operator double()
{
    if (m_type != json_double)
    {
        throw new logic_error("type error, not double value");
    }
    return m_value.m_double;
}
Json::operator string()
{
    if (m_type != json_string)
    {
        throw new logic_error("type error, not string value");
    }
    return *(m_value.m_string);   //因为存放的是指针，所以进行解引用
}


//数组重载
Json & Json::operator []  (int index)
{
    if (m_type != json_array)   //如果不是数组类型，就先准换为数组类型
    {
        clear();
        m_type = json_array;
        m_value.m_array = new vector<Json>();
    }
    if (index < 0)
    {
        throw new logic_error("array[] index < 0");
    }
    int size = (m_value.m_array)->size();     //获取数组的大小
    if (index >= size)                        //如果大于数组的大小，说明需要扩容了
    {
        for (int i = size; i <= index; i ++)
        {
            (m_value.m_array)->push_back(Json());   //扩容就是在后面插入Json
        }
    }
    return   m_value.m_array->at(index);                    
}
void Json::append(const Json& other)
{
    if (m_type != json_array)   //如果不是数组类型，就先准换为数组类型
    {
        clear();
        m_type = json_array;
        m_value.m_array = new vector<Json>();
    }
    (m_value.m_array)->push_back(other);
}


//结构体重载
Json & Json::operator []  (const char * key)
{
    string name(key);
    return (*(this))[name];  //直接调用第二个重载的
}
Json & Json::operator []  (const string& key)
{
    if (m_type != json_object)
    {
        clear();
        m_type = json_object;
        m_value.m_object = new std::map<string, Json>();   
    }
    //判断key是否在map里面
    return (*(m_value.m_object))[key];
}


//=操作符重载
void Json::operator = (const Json& other)
{
    clear();
    //和前面拷贝构造的运算符一样
    copy(other);
}

bool Json::operator == (const Json& other)
{
    if (m_type != other.m_type)
    {
        return false;
    }
    switch(m_type)
    {
        case json_null:
            return true;
        case json_bool:
            return m_value.m_bool == other.m_value.m_bool;
        case json_int:
            return m_value.m_int == other.m_value.m_int;
        case json_double:
            return m_value.m_double == other.m_value.m_double;
        case json_array:
            return m_value.m_array == other.m_value.m_array;
        case json_string:
            return *(m_value.m_string) == *(other.m_value.m_string);
        case json_object:
            return m_value.m_object == other.m_value.m_object;
        default:
            break;
    }
    return false;
}
bool Json::operator != (const Json& other)
{
    return !((*this) == other);
}

//copy
void Json::copy(const Json & other)
{
    m_type = other.m_type;
    switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = other.m_value.m_bool;
            break;
        case json_int:
            m_value.m_int =  other.m_value.m_int;
            break;
        case json_double:
            m_value.m_double =  other.m_value.m_double;
            break;
        case json_string:
            m_value.m_string = other.m_value.m_string;
            break;
        case json_array:
            m_value.m_array = other.m_value.m_array;
            break;
        case json_object:
            m_value.m_object = other.m_value.m_object;
            break;
        default:
            break;
    }
}


// clear，防止内存泄露
void Json::clear()
{
        switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = false;
            break;
        case json_int:
            m_value.m_int =  0;
            break;
        case json_double:
            m_value.m_double =  0.0;
            break;
        case json_string:
            {
                delete m_value.m_string;
                break;
            }
            break;
        case json_array:
            {   //先清楚元素，在delete内存
                for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it ++)
                {
                    it->clear();
                }
                delete m_value.m_array;
                break;
            }
            
        case json_object:
            {   //先清楚元素，在delete内存
                for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it ++)
                {
                    (it->second).clear();
                }
                delete m_value.m_object;
                break;
            }
        default:
            break;
    }
    m_type = json_null;  // 清理完，将类型改为空类型
}

//将arr中的内容打印出来
string Json::str() const
{
    stringstream ss;   //
    switch(m_type)
    {
        case json_null:
            ss << "null";   //将字符串 "null" 插入到 ss 中 || // 将字符 '[' 插入到 ss 中
            break;
        case json_bool:
            if (m_value.m_bool)
            {
                ss << "true";
            }else{
                ss << "false";
            }
            break;
        case json_int:
            ss << m_value.m_int;
            break;
        case json_double:
            ss << m_value.m_double;
            break;
        case json_string:
            ss << '\"' << *(m_value.m_string) << '\"';
            break;
        case json_array:
            {
                ss << '[';
                for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it ++)
                {
                    if (it != m_value.m_array->begin())
                    {
                        ss << ',';    //数组中元素与元素之间使用,分割
                    }
                    ss << it->str();  //递归调用
                }
                ss << ']';
                break;
            }
        case json_object:
            {
                ss << '{';
                for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it ++)
                {
                    if (it != m_value.m_object->begin())
                    {
                        ss << ',';    //数组中元素与元素之间使用,分割
                    }
                    ss << '\"' << it->first << '\"' << ':' << it->second.str();  //对象里面是键值对调用key放在first里面，而且是个字符串，使用""包住
                }
                ss << '}';
                break;
            }
        default:
            break;
    }
    return ss.str();
}


bool Json::asBool() const
{
    if (m_type != json_bool)
    {
        throw new std::logic_error("type error, not bool value");   
    }
    return m_value.m_bool;
}
int Json::asInt() const
{
    if (m_type != json_int)
    {
        throw new std::logic_error("type error, not int value");   
    }
    return m_value.m_int;
}
double Json::asDouble() const
{
    if (m_type != json_double)
    {
        throw new std::logic_error("type error, not double value");   
    }
    return m_value.m_double;
}
string Json::asString() const
{
    if (m_type != json_string)
    {
        throw new std::logic_error("type error, not string value");   
    }
    return *(m_value.m_string);
}

bool Json::has(int index)
{
    if (m_type != json_array)
    {
        return false;
    }
    int size = (m_value.m_array)->size();
    return (index >= 0 && index < size);
}
bool Json::has(const char * key)
{
    string name(key);
    return has(name);

}
bool Json::has(const string& key)
{
    if (m_type != json_object)
    {
        return false;
    }
    return ((m_value.m_object)->find(key) != m_value.m_object->end());
}

void Json::remove(int index)
{
    if (m_type != json_array)
    {
        return;
    }
    int size = m_value.m_array->size();
    if (index < 0 || index >= size)
    {
        return;
    }
    (m_value.m_array)->at(index).clear();
    (m_value.m_array)->erase((m_value.m_array)->begin() + index);
}
void Json::remove(const char* key)
{
    string name(key);
    remove(name);  //直接调用下面这个
}
void Json::remove(const string & key)
{
    auto it = m_value.m_object->find(key);
    if (it == (m_value.m_object)->end())
    {
        return;
    }
    ((*m_value.m_object))[key].clear();
    (m_value.m_object)->erase(key);
}

void Json::parse(const string & str)
{
    Parser p;
    p.load(str);
    *this = p.parse();
}