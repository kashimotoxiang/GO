var ws;
var connect_flag = false;
var myJSON;
//num2str调整格式
function num2strfix(num, length) {
    return ('' + num).length < length ? ((new Array(length + 1)).join('0') + num).slice(-length) : '' + num;
}

function websocketInit() {
    ws = new WebSocket("ws://localhost:8181");
    ws.binaryType = "arraybuffer";//set the type of received data:array, the default type is bolb
    ws.onopen = function (event) {
        console.log("Connection Start");
    };

    ws.onmessage = function (event) {
        connect_flag = true;
        if (event.data instanceof ArrayBuffer) {//验证格式是否正确
            var rArray = new Int32Array(event.data); //Int32Array cannot be changed if created
            console.log(rArray);
            //验证广播first name
            var i_name = rArray.subarray(0, 29);
            master = nameExtract(i_name);
            var _pan = rArray.subarray(30);

            // //提取second name
            // var i_name = rArray.subarray(30, 60);
            // rival = nameExtract(i_name);

            // // var rival = document.getElementById("rival");
            // if (rival !== "")
            //     $("#rival").val("您的对手: " + rival);
            // else
            //     $("#rival").val("单机");
            // var _pan = rArray.subarray(60);


            if (master === g_name) {
                pan = _pan;
                showPan();
                console.log("is me");
            }
            else {
                console.log("not me");
            }
        }
        else {
            console.log("Receive:" + event.data);
        }
        $("#step").val("步数：" + move_count);//显示步数
    };

    ws.onclose = function (event) {
        console.log("Connection Stopped");
        connect_flag = false;
    }
}

function websocketShutdown() {
    var myJSON = JSON.stringify({ "key": "stop", "name": g_name, "mode": mode });
    sendMessage(myJSON);
    ws.close();
}
//send message to server
function sendMessage(data) {
    console.log("Send:" + data);
    ws.send(data);
}

function sendServer(x, y, move_count) {
    xs = num2strfix(x, 2);
    ys = num2strfix(y, 2);

    var myJSON = JSON.stringify({ "key": "play", "name": g_name, "mode": mode, "row": xs, "col": ys });
    sendMessage(myJSON);
}

function nameExtract(i_name) {
    var master = [];
    for (var i in i_name) {
        if (i_name[i] !== -1) {
            var code = String.fromCharCode(i_name[i]);
            master.push(code);
        }
    }
    master = master.join("");
    return master;
}

addLoadEvent(websocketInit);
