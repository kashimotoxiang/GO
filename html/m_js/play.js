function showPan() {
    var c = document.getElementById("board");
    var cxt = c.getContext("2d");
    cxt.strokeStyle = "black";

    /* 清空，重新画线等 */
    cxt.clearRect(0, 0, 600, 600);
    cxt.fillStyle = "sandybrown";
    cxt.fillRect(0, 0, 600, 600);
    grid(cxt);
    ninePoints(cxt);
    var temp;
    for (var i = 0; i < 19; i++) {
        for (var j = 0; j < 19; j++) { //画棋子
            temp = pan[i * 19 + j];
            if (temp === 1) { //black
                var rg = cxt.createRadialGradient((i + 1) * 30 - 3, (j + 1) * 30 - 3, 1, (i + 1) * 30 - 4, (j + 1) * 30 - 4, 11);
                rg.addColorStop(1, /*"black"*/ "#202020");
                rg.addColorStop(0, "gray");
                cxt.beginPath();
                cxt.arc((i + 1) * 30, (j + 1) * 30, 15, 0, 2 * Math.PI, false);
                //cxt.fillStyle="black";
                cxt.fillStyle = rg;
                cxt.fill();

            } else if (temp === 2) { //white
                var rg = cxt.createRadialGradient((i + 1) * 30 - 3, (j + 1) * 30 - 3, 1, (i + 1) * 30 - 4, (j + 1) * 30 - 4, 11);
                rg.addColorStop(1, /*"lightgray"*/ "#e0e0e0");
                rg.addColorStop(0, "white");
                cxt.beginPath();
                cxt.arc((i + 1) * 30, (j + 1) * 30, 15, 0, 2 * Math.PI, false);
                //cxt.fillStyle="white";
                cxt.fillStyle = rg;
                cxt.fill();
            } else if (temp === 7) { // fill color
                cxt.beginPath();
                cxt.arc((i + 1) * 30, (j + 1) * 30, 15, 0, 2 * Math.PI, false);
                cxt.fillStyle = "red";
                cxt.fill();
            }
        }
    }
}
