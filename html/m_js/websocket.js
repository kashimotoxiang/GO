var ws;
var START_FLAG = 0;
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
        myJSON = JSON.stringify({ "key": "start", "name": g_name });
        sendMessage(myJSON);
        START_FLAG = 1;
    };

    ws.onmessage = function (event) {
        if (event.data instanceof ArrayBuffer) {//验证格式是否正确
            var rArray = new Int32Array(event.data); //Int32Array cannot be changed if created
            console.log(rArray);

            //验证广播名字
            var i_name = rArray.subarray(0, 30);
            var s_name = [];

            for (var i in i_name) {
                if (i_name[i] !== -1) {
                    var code = String.fromCharCode(i_name[i]);
                    s_name.push(code);
                }
            }
            s_name = s_name.join("");

            var _pan = rArray.subarray(30);

            if (s_name === g_name) {
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
        START_FLAG = 0;
    }
}

function websocketShutdown() {
    var myJSON = JSON.stringify({ "key": "stop", "name": g_name });
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

    var myJSON = JSON.stringify({ "key": "play", "name": g_name, "row": xs, "col": ys });
    sendMessage(myJSON);
}


addLoadEvent(websocketInit);
