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
        myJSON = JSON.stringify({"key": "start"});
        sendMessage(myJSON);
        START_FLAG = 1;
    };

    ws.onmessage = function (event) {
    if (event.data instanceof ArrayBuffer) {//验证格式是否正确
    var rArray = new Int32Array(event.data); //Int32Array cannot be changed if created
    pan=rArray;
    showPan();
    console.log(rArray);
}
else console.log("Receive:" + event.data);
    $("#step").val("步数："+move_count);//显示步数
};

ws.onclose = function (event) {
    console.log("Connection Stopped");
    START_FLAG = 0;
}
}

function websocketShutdown() {
    var myJSON = JSON.stringify({"key":"stop"});
    sendMessage(myJSON);
    ws.close();
}
//send message to server
function sendMessage(data) {
    console.log("Send:" + data);
    ws.send(data);
}

function sendServer(x, y, move_count){
    xs=num2strfix(x, 2);
    ys=num2strfix(y, 2);

    var myJSON = JSON.stringify({"key": "play", "row": xs, "col": ys});
    sendMessage(myJSON);
}

// addLoadEvent(websocketInit);
