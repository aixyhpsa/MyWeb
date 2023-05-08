let accountNumber = document.querySelector(".accountNumber");
let password = document.querySelector(".password");
let login_btn = document.querySelector("#login_btn");

login_btn.addEventListener("click", function(){
    let xhr = new XMLHttpRequest();
    let sendBuff = '{"accountNumber":"' + accountNumber.value + '", "password":"' + password.value + '"}';
    alert(sendBuff);
    xhr.open("POST", "http://dx191dya.com/post");
    xhr.send(sendBuff);
    xhr.onreadystatechange = function(){
        if (xhr.readyState === 4)
        {
            if (xhr.status >= 200 && xhr.status < 300)
            {
                //if (xhr.response)
                alert(xhr.response);
                let form = document.querySelector(".login");
                form.style.display = "none";
            }
        }
    }
});
