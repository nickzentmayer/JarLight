
var gateway = `ws://${window.location.hostname}/ws`;
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




/*<input type="button" value="Cylon" class="animButtons" onclick="sendMsg('a:cylon')">
            <input type="button" value="Cycle" class="animButtons" onclick="sendMsg('a:cycle')">
            <input type="button" value="Fall" class="animButtons" onclick="sendMsg('a:fall')">
            <input type="button" value="Halloween" class="animButtons" onclick="sendMsg('a:halloween')">*/