function delRow(btn){
    var tab=document.getElementById("myTable"); 
    //btn:表示点击的当前的删除按钮， 
    //btn.parentNode获取的按钮的父元素td,btn.parentNode.parentNode获取的按钮的父元素td的父元素tr
    var trIndex=btn.parentNode.parentNode.rowIndex;
    //根据行的下标索引删除行
    tab.deleteRow(trIndex);
}

function addOneRow(filename, date, type, size){ //获取到表格的对象
    var tab=document.getElementById("myTable");
    //给表格添加一行
    let newTr=tab.insertRow();//创建了一个空行，在页面上看不出来 //给新创建的行添加列
    var newTd1=newTr.insertCell();//给新的行创建了一个新的单元格
    var newTd2=newTr.insertCell();
    var newTd3=newTr.insertCell();
    var newTd4=newTr.insertCell();
    var newTd5=newTr.insertCell();
    //给每一个单元格中添加内容
    newTd1.innerHTML=filename;
    newTd2.innerHTML=date;
    newTd3.innerHTML=type;
    newTd4.innerHTML=size;
    newTd5.innerHTML='<button type="button" onclick="delRow(this)">删除</button>'; 
    // 鼠标移动变色
    newTr.addEventListener("mouseenter", function(){
        this.style.color= "pink";
    });
    newTr.addEventListener("mouseleave", function(){
        this.style.color= "black";
    });
}

// 页面加载事件
document.addEventListener("DOMContentLoaded", function(){
    let xhr = new XMLHttpRequest();
    xhr.open("POST", "http://dx191dya.com/post");
    xhr.send('{"showList":"rootPath"}');
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
    };
});

let upload = document.querySelector(".upload");
upload.addEventListener("click", function(){
    let form = document.querySelector(".select");
    let fd = new FormData(form);
    let xhr = new XMLHttpRequest();
    xhr.open("POST", "http://dx191dya.com/post");
    xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    xhr.send(fd);
    xhr.onreadystatechange = function(){
        if (xhr.readyState === 4)
        {
            if (xhr.status >= 200 && xhr.status < 300)
            {
                alert(xhr.response);
            }
        }
    }
});