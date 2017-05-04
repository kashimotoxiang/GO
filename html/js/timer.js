var interval, reg = /^\d$/,
    sleep = 10,
    n = 0;

function clock() {
    var timer_button = document.getElementById("oStart");
    if (!interval) {
        interval = setInterval(function () {
            clearInterval(interval);
            interval = setInterval(function () {
                n++;
                var s = parseInt(n % 60);
                var m = parseInt((n / 60) % 60);
                var h = parseInt((n / 3600) % 60);
                $("#oTxt").val("时间：" + toDub(h) + ":" + toDub(m) + ":" + toDub(s));
            }, 1000 / 60);
        }, sleep);
        timer_button.innerHTML = "暂停计时";
    } else {
        clearInterval(interval);
        interval = null;
        timer_button.innerHTML = "开始计时";
    }
}

//补零
function toDub(n) {
    return n < 10 ? "0" + n : "" + n;
}


