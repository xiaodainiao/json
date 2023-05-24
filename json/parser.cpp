#include "parser.h"
using namespace  xiaodainiao::json;

Parser::Parser(): m_str(""), m_idx(0)
{
    
}
void Parser::load(const string& str)
{
    m_str = str;
    m_idx = 0;
}

//使用递归的技巧，将空白字符，回车，空行，换行等忽略掉
void Parser::skip_write_space()
{
    while(m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\r' || m_str[m_idx] == '\t')
    {
        m_idx ++;
    }
}

char Parser::get_next_token()
{   
    skip_write_space();
    char ch = m_str[m_idx];   //首先获取当前字符的索引，然后++,最后返回
    m_idx ++;
    return ch;
}

Json Parser::parse()
{
    char ch = get_next_token();
    switch (ch)
    {
    case 'n':
        m_idx --;
        return parse_null();
    case 't':
    case 'f':
        m_idx --;
        return parse_bool();
    case '-':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        m_idx --;
        return parse_number();
    case '"': //如果以双引号开始则认为是字符串
        return Json(parse_string());
    case '[':
        return parse_array();
    case '{':
        return parse_object();
    default:
        break;
    }
    throw new std::logic_error("unexpected char");
}

Json Parser::parse_null()
{
    if (m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return Json();
    }
    throw new std::logic_error("parse null error!");
}
Json Parser::parse_bool()
{
    if (m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return Json(true);
    }else if (m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return Json(false);
    }
    throw new std::logic_error("parse bool error!");

}
Json Parser::parse_number()
{
    int pos = m_idx;   //先记录原始位置
    if (m_str[m_idx] == '-')
    {
        m_idx ++;
    }
    if (m_str[m_idx] < '0' || m_str[m_idx] > '9')
    {
        throw new std::logic_error("parse number error!");
    }

    while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
    {
        m_idx++;
    }
    //判断是否是有小数点，有的话为浮点数
    if (m_str[m_idx] != '.')
    {
        int i = std::atoi(m_str.c_str() + pos);
        return Json(i);
    }

    m_idx ++;  //如果是.就继续往后移
    if (m_str[m_idx] < '0' || m_str[m_idx] > '9')
    {
        throw new std::logic_error("parse number error!");
    }

    while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
    {
        m_idx++;
    }
    double f = std::atof(m_str.c_str() + pos);

    return Json(f);
}
string Parser::parse_string()
{
    string out;
    while (true)
    {
        char ch = m_str[m_idx++];
        if (ch == '"')  //说明字符串结束
        {
            break;
        }
        if (ch == '\\')
        {
            ch = m_str[m_idx ++];
            switch (ch)
            {
            case '\n':
                out += '\n';
                break;
            case '\r':
                out += '\r';
                break;
            case '\t':
                out += '\t';
                break;
            case '\b':
                out += '\b';
                break;
            case '\f':
                out += '\f';
                break;
            case '"':
                out += "\\\"";
                break;
            case '\\':
                out += "\\\\";
                break;
            case 'u':
                out += "\\u";
                for (int i = 0; i < 4; i ++)
                {
                    out += m_str[m_idx++];
                }
                break;

            default:
                break;
            }
        }
        else    //如果不是转义字符直接+
        {
            out += ch;
        }
    }
    return out;
}
Json Parser::parse_array()
{
    //声明对象类型
    Json arr(Json::json_array);  //创建一个名为 arr 的 Json 对象，并将其类型设置为 JSON 数组类型。
    char ch = get_next_token();
    if (ch == ']')
    {
        return arr;
    }
    m_idx --;
    while (true)
    {
        arr.append(parse());  //通过递归调用，解析它的元素，可能是数组，bool、结构体等
        ch = get_next_token();
        if (ch == ']')
        {
            break;
        }
        if (ch != ',')
        {
            throw new std::logic_error("parse array error !");
        }
        m_idx++;
    }
    return arr;
}
Json Parser::parse_object()
{
    Json obj(Json::json_object);
    char ch = get_next_token();
    if (ch == '}')
    {
        return obj;
    }
    m_idx --;
    while(true)
    {
        ch = get_next_token();
        if (ch != '"')   //因为对象是键值对，key是字符串由"开头
        {
            throw new std::logic_error("parse object error");
        }
        string key = parse_string();
        ch = get_next_token();
        if (ch != ':')
        {
            throw new std::logic_error("parse object error");
        }
        obj[key] = parse();
        ch = get_next_token();
        if (ch == '}')
        {
            break;
        }
        if (ch != ',')
        {
            throw new std::logic_error("parse object error !");
        }
        m_idx++;
    }
    return obj;
}