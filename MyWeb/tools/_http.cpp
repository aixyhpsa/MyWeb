#include "_http.h"

namespace dya 
{
////////////////////////////////////////////////////////////////////
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
////////////////////////////////////////////////////////////////////
Http::Http() :
m_parser(std::make_unique<http_parser>()),
m_setting(std::make_unique<http_parser_settings>()),
m_result(32), m_errno(nullptr)
{
    http_parser_init(m_parser.get(), HTTP_REQUEST);
    m_setting->on_message_begin = std::bind(&Http::on_message_begin, this, _1);
    m_setting->on_headers_complete= std::bind(&Http::on_headers_complete, this, _1);
    m_setting->on_message_complete= std::bind(&Http::on_message_complete, this, _1);
    m_setting->on_url = std::bind(&Http::on_url, this, _1, _2, _3);
    m_setting->on_header_field = std::bind(&Http::on_header_field, this, _1, _2, _3);
    m_setting->on_header_value = std::bind(&Http::on_header_value, this, _1, _2, _3);
    m_setting->on_body = std::bind(&Http::on_body, this, _1, _2, _3);
}

Http::~Http()
{
    m_result.clear();
}

bool Http::parser(const std::string &buff)
{
    int parserd = http_parser_execute(m_parser.get(), m_setting.get(), buff.c_str(), buff.size());
    if (parserd < buff.size())
    {
        m_errno = http_errno_description(HTTP_PARSER_ERRNO(m_parser.get()));
        return false;
    }
    return true;
}

void Http::reset(int type)
{
    http_parser_init(m_parser.get(), (enum http_parser_type)type);
    m_result.clear();
}

std::string& Http::operator[](const std::string &key)
{
    return m_result[key];
}


const char* Http::error()
{
    return m_errno;
}

bool Http::find(const std::string &key)
{
    return m_result.find(key) == m_result.end() ? false : true;
}

int Http::on_message_begin(http_parser* _) { return 0; }
int Http::on_headers_complete(http_parser* _) { return 0; }
int Http::on_message_complete(http_parser* _) { return 0; }

int Http::on_url(http_parser* _, const char* at, size_t length)
{
    m_result.emplace("url", std::string(at, length));
    return 0;
}
int Http::on_header_field(http_parser* _, const char* at, size_t length)
{
    m_result.emplace(std::string(at, length), "");
    _->data = (void*)(m_result.find(std::string(at, length))->first.c_str());
    return 0;
}
int Http::on_header_value(http_parser* _, const char* at, size_t length)
{
    m_result[(char*)_->data] = std::string(at, length);
    return 0;
}
int Http::on_body(http_parser* _, const char* at, size_t length)
{
    m_result.emplace("body", std::string(at, length));
    return 0;
}

int Http::on_status(http_parser* _, const char* at, size_t length)
{
    m_result.emplace("status", std::string(at, length));
    return 0;
}
////////////////////////////////////////////////////////////////////
}
