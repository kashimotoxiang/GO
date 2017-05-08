"use strict";

var start_flag = false;
function Button1() {
    var start_button = document.getElementById("start_connect");
    if (start_button) {
        start_button.onclick = function () {
            if (start_flag) {
                start_button.innerHTML = "开始游戏";
                var myJSON = JSON.stringify({ "key": "stop", "name": g_name });
                sendMessage(myJSON);
                timerShut();
                // websocketShutdown();//停止
                start_flag = false;
            } else {
                start_button.innerHTML = "终止游戏";
                var myJSON = JSON.stringify({ "key": "start", "name": g_name });
                sendMessage(myJSON);
                timerBegin();
                // websocketInit();//启动
                start_flag = true;
            }
        }
    }
}

function getCookie(cname) {
    var name = cname + "=";
    var ca = document.cookie.split(';');
    for (var i = 0; i < ca.length; i++) {
        var c = ca[i].trim();
        if (c.indexOf(name) == 0) return c.substring(name.length, c.length);
    }
    return "";
}

function background() {
    g_name = getCookie("g_name");
    var welcome = document.getElementById("welcome");
    $("#welcome").val("欢迎 " + g_name);
}

addLoadEvent(Button1);
addLoadEvent(background);

