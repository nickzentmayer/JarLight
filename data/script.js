
var gateway = `ws://${window.location.hostname}:${window.location.port}/ws`;
var websocket;
var animButtons;
var animCount = 0;
function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage; // <-- add this line
}
function onOpen(event) {
    console.log('Connection opened');
    //sendMsg('upd');
    let len = animButtons.getElementsByClassName("animButtons").length
    for(let i = 0; i < len; i++) animButtons.getElementsByClassName("animButtons")[0].remove();
    animCount = 0;
    sendMsg('getAnimations');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
    console.log(event.data);
    var topic = event.data.toString().substring(0, event.data.toString().indexOf(':'));
    var value = event.data.toString().substring(event.data.toString().indexOf(':') + 1);
    console.log(topic + value)
    if (topic == 'p') if (value == "1") document.getElementById('powerSwitch').checked = true;
    if (topic == 'c') document.getElementById('colorpicker').value = value;
    if (topic == 'b') document.getElementById('bright').value = value;
    if (topic == 's') document.getElementById('speed').value = value;
    if (topic == 'batt') document.getElementById('batP').innerHTML = value + "%";
    if (topic == 'w') {
        if(value == 'AP') document.getElementById('recon').style.display = "inline";
        if(value == 'STA') document.getElementById('recon').style.display = "none";
    }
    if (topic == 'n') document.getElementById('deviceName').innerHTML = value;
    if(topic == 'a') {
        var b = document.createElement("button");
        b.value = (animCount++).toString();
        b.className = "animButtons";
        b.onclick = function() {sendMsg("a:" + this.value)};
        b.textContent = value;
        animButtons.appendChild(b);
    }
    if(topic == 'type') {
        if(value == 'battery') {
            document.getElementById('battery').style.display = "inline";
            document.getElementById('sleep').style.display = "inline";
        }
        if(value == 'wall') {
            document.getElementById('battery').style.display = "none"; 
            document.getElementById('sleep').style.display = "none";
        }
    }
}
window.addEventListener('load', onLoad);

function sendMsg(msg) {
    websocket.send(msg);
}

function onLoad(event) {
    initWebSocket();
    //document.getElementById('animButtons').onpointerover = changeColor();
    animButtons = document.getElementById("animations");
}

function sendTimer() {
    if(document.getElementById("onTime").value != "" && document.getElementById("offTime").value != "") {
    console.log(document.getElementById("onTime").value);
    console.log(document.getElementById("offTime").value);
    sendMsg('t:on:'+document.getElementById("onTime").value.toString());
    sendMsg('t:off:'+document.getElementById("offTime").value.toString());
    }
    else {
        if(document.getElementById("onTime").value == "") invalidFlash("onTime");
        if(document.getElementById("offTime").value == "") invalidFlash("offTime");
    }
}
async function invalidFlash(id) {
    let elem = document.getElementById(id);
    elem.style.color = 'red';
    setTimeout(() => { elem.style.color = 'white' }, 2000);
}



/*<input type="button" value="Cylon" class="animButtons" onclick="sendMsg('a:cylon')">
            <input type="button" value="Cycle" class="animButtons" onclick="sendMsg('a:cycle')">
            <input type="button" value="Fall" class="animButtons" onclick="sendMsg('a:fall')">
            <input type="button" value="Halloween" class="animButtons" onclick="sendMsg('a:halloween')">*/