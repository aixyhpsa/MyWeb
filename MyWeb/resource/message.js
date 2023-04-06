//1、获取元素
let txt = document.querySelector(".message textarea");
let btn = document.querySelector(".message button");
let ul = document.querySelector(".message ul");
function addMessage(content) {
    //创建元素li作为留言区
    let li = document.createElement("li");
    //将文本框中的内容放到li中,a标签中href中的内容代表什么都不执行
    li.innerHTML = content + "<a href='javascript:;'>删除</a>"
    txt.value = "";//添加之后文本框的内容清空
    //将留言添加到ul中，而且在最上方
    ul.insertBefore(li, ul.children[0]);
    //当点击删除时删除ul中的li
    let as = document.querySelectorAll("a");
    //循环给每个删除绑定事件
    for (let i = 0; i < as.length; i++) {
        as[i].onclick = function () {
            //删除当前a标签所在位置的父节点li
            ul.removeChild(this.parentNode);
        }
    }
}
//2、给button绑定点击事件
btn.onclick = function () {
    if (txt.value == "") 
    {
        //当文本框中没有文字时，提示
        alert("您没有输入内容")
    } 
    else 
    {
        let meg = "{\"Message_add\":" + "\"" + txt.value + "\"" + "}";
        let xhr = new XMLHttpRequest();
        xhr.open("POST", "http://dx191dya.com/post");
        xhr.send(meg);
        xhr.onreadystatechange = function(){
            if (xhr.readyState === 4)
            {
                if (xhr.status >= 200 && xhr.status < 300)
                {
                    // 解析json

                    addMessage();
                }
                else
                {
                    alert("发布失败，服务器异常");
                }
            }
        }
    }
}

// 窗口加载事件
window.addEventListener("load", function(){
    let xhr = new XMLHttpRequest();
    xhr.open("POST", "http://dx191dya.com:5099/post");
    xhr.send('{"message_show":"all"}');
    xhr.onreadystatechange = function(){
        if (xhr.readyState === 4)
        {
            // status是响应代码
            if (xhr.status >= 200 && xhr.status < 300)
            {
                // response是响应体
                let ret = JSON.parse(xhr.response);
                for (let k in ret)
                {
                    addOneRow(k, ret[k].date, ret[k].type, ret[k].size);
                }
            }
        }
        else
        {
            console.log("加载留言失败");
        }
    };
});