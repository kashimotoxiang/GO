var ws;
function websocketInit() {
  ws = new WebSocket("ws://localhost:8181");
  ws.binaryType = "arraybuffer";//set the type of received data:array, the default type is bolb
  ws.onopen = function (event) {
    console.log("Connection Start");
  };

  ws.onmessage = function (event) {
    mReceive(event.data);
  };

  ws.onclose = function (event) {
    console.log("Connection Stopped");
  }
}

function sendMessage(data) {
  console.log("Send:" + data);
  ws.send(data);
}

function setCookie(cname, cvalue, exdays) {
  var d = new Date();
  d.setTime(d.getTime() + (exdays * 24 * 60 * 60 * 1000));
  var expires = "expires=" + d.toGMTString();
  document.cookie = cname + "=" + cvalue + "; " + expires;
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

function mReceive(message) {
  var m = message.substr(0, message.length - 1);
  switch (m) {
    // case OVERFLOW:alert("超出在线人数！最多同时在线2人！请等待其他人退出！");break;
    case "NAMEEXIST": alert("用户名已存在！"); break;
    case "EMAILEXIST": alert("邮箱已存在！"); break;
    case "NAMEERROR": alert("用户名或邮箱错误！"); break;
    case "PASSWORDERROR": alert("密码错误！"); break;
    case "SIGNUPOK": alert("注册成功！"); break;
    case "LOGINOK": {
      // var dt = new Date();
      // dt.setSeconds(dt.getSeconds() + 60);
      // document.cookie = "cookietest=1; expires=" + dt.toGMTString();
      // var cookiesEnabled = document.cookie.indexOf("cookietest=") != -1;
      // if (!cookiesEnabled) {
      //   //没有启用cookie   
      //   alert("没有启用cookie ");
      // } else {
      //   //已经启用cookie   
      //   alert("已经启用cookie ");
      // }

      var i_name = document.getElementById('username1');
      var name = i_name.value;

      if (name != "" && name != null) {
        setCookie("g_name", name, 300);
      }
      var user = getCookie("username");

      window.location.href = 'file://C:/Users/kashi/Dropbox/02大学/大三（下）/软件设计/WEIQI/html/main.html';
      break;
    }
  }
}

function addLoadEvent(func) {
  var oldonload = window.onload;
  if (typeof window.onload != 'function') {
    window.onload = func;
  } else {
    window.onload = function () {
      oldonload();
      func();
    }
  }
}

//初始化websocket
addLoadEvent(websocketInit);
