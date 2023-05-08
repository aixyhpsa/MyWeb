## server
1. 登录(login)
成功：
    "登录成功"
失败：
    "error"


2. 上传(upload)
成功：
    "上传成功"
失败：
    "error"


3. 展示文件列表(show)
成功：
    {
        "/home/wucz/..." : {"size" : xx, "date" : "xxx", "type" : "dir"},
        "/home/wucz/..." : {"size" : xx, "date" : "xxx", "type" : "dir"}
    }
失败：
    "error"


4. 留言板(message)
成功：
    {
        "1" : {"time" : "xxx", "message" : "xxx"}
    }
失败：
    "error"


## client
1. 登录(login)
{
    "accountNumber" : "xxx", 
    "password" : "xxx"
}

2. 上传(upload)
"----WebKitFormBoundaryi"

3. 展示文件列表(show)
{
    "showList" : "rootPath"
}

4. 留言板(message)
{
    "message_add" : "留言内容";
}

{
    "message_show" : "all";
}