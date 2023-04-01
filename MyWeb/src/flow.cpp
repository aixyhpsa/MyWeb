#include "flow.h"
#include "login.h"

namespace dya
{
////////////////////////////////////////////////////////////////////
Flow::Flow(std::string &buff) : m_buff(buff) {}

Flow::Flow(std::string &&buff) : m_buff(std::move(buff)) {}

std::string Flow::handle()
{
    // 1 GET? POST?
    // dya::Http http.parser(m_buff);
    // 1.1 GET
    // 1.1.1 展示文件列表? 展示留言? 获取资源?
    // 1.1.2 获取资源
    // dya::Get gg();
    // return gg.getResponse();
    // 1.1.3 展示文件列表

    // 1.2 POST
    // 1.2.1 上传? 登录? 留言? 
    // 1.2.2 登录 
    // {"accountNumber":"root","password":"368751429QQ"}
    // nlohmann::json jj = json.parser();
    // dya::Login login(SQL);
    // bool ret = login(accountNumber, password);
    // dya::Post pp(XXX);

    // 1.2.3 上传
    // 判断是否登录(cookie)
    // 解析body
    // 本地创建文件，写入body
    // dya::ShowList sl(NEW);
    // std::string ret = s1.getList();
    // dya::Post pp(xxx);

    // 1.2.4 留言
    // {"messageBoard":"XXX"}
    // nlohmann::json jj = json.parser();
    // 登录数据库
    // 执行SQL
    // dya::ShowMessage sm(NEW);
    // std::string ret = sm.getMessage();
    // dya::Post pp(xxx);
}
////////////////////////////////////////////////////////////////////
}