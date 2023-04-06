## server
1. 登录(login)
"登录成功"

2. 上传(upload)
"上传成功"

3. 展示文件列表(show)
{
    "/home/wucz/..." : {"size" : xx, "date" : "xxx", "type" : "dir"},
    "/home/wucz/..." : {"size" : xx, "date" : "xxx", "type" : "dir"}
}

4. 留言板(message)
{
    "1" : {"time" : "xxx", "message" : "xxx"}
}

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