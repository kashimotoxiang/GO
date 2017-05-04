/*
 author: @nixzhu (zhuhongxu@gmail.com)
 description: 配置手数显示等辅助功能，TODO 撤销、同色等
 license: GPL
 */

 "use strict";

// //启动&停止
// function Button1() {
//     var str1, str2,str3, m_switch;
//     m_switch = $('#start_connect');
//     str1 = "开始连接";
//     str2 = "终止连接";
//     str3=m_switch.val();
//     if (str3 == str1) {
//         m_switch.val(str2);
//         websocketInit();//启动
//     }
//     else {
//         m_switch.val(str1);
//         websocketShutdown();//停止
//     }
// }
// $(document).ready(function () {
//     var button;
//     button = $('#start_connect');
//     button.on('click', function () {
//         Button1();
//     });
// });

var start_flag = false;
function Button1() {
    var start_button = document.getElementById("start_connect");
    if (start_button) {
        start_button.onclick = function () {
            if (start_flag) {
                start_button.innerHTML = "开始连接";
                websocketShutdown();//停止
                start_flag = false;
            } else {
                start_button.innerHTML = "终止连接";
                websocketInit();//启动
                start_flag = true;
            }
        }
    }
}


var move_show_flag = false;
function deal_button() {
    var move_show_button = document.getElementById("move_show");
    if (move_show_button) {
        move_show_button.onclick = function () {
            if (move_show_flag) {
                move_show_button.innerHTML = "显示手数";
                move_show_flag = false;
            } else {
                move_show_button.innerHTML = "取消显示手数";
                move_show_flag = true;
            }
            showPan();
        }
    }
}
addLoadEvent(Button1);
addLoadEvent(deal_button);

