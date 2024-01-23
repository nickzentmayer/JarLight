
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
    if(animButtons != null) {
    let len = animButtons.getElementsByClassName("animButtons").length
    for(let i = 0; i < len; i++) animButtons.getElementsByClassName("animButtons")[0].remove();
    animCount = 0;
    sendMsg('getAnimations');
    }
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}
function onMessage(event) {
    console.log(event.data);
    updatePage(event.data);
}
function updatePage(data) {
    var topic = data.toString().substring(0, data.toString().indexOf(':'));
    var value = data.toString().substring(data.toString().indexOf(':') + 1);
    console.log(topic + value)
    if (topic == 'p') if (value == "1") document.getElementById('powerSwitch').checked = true;
    else document.getElementById('powerSwitch').checked = false;
    if (topic == 'c') document.getElementById('colorpicker').value = value;
    if (topic == 'b') document.getElementById('bright').value = value;
    if (topic == 's') document.getElementById('speed').value = value;
    if (topic == 'batt') document.getElementById('batP').innerHTML = value + "%";
    if (topic == 'w') {
        if(value == 'AP') document.getElementById('recon').style.display = "inline";
        if(value == 'STA') document.getElementById('recon').style.display = "none";
    }
    if (topic == 'n') document.getElementById('deviceName').innerHTML = value;
    if(topic == 'a' && animButtons != null) {
        for(let i = 0; i < animButtons.getElementsByClassName("animButtons").length; i++) if(animButtons.getElementsByClassName("animButtons")[i].textContent == value) return;
        var b = document.createElement("button");
        b.value = (animCount++).toString();
        b.className = "animButtons";
        b.onclick = function() {sendMsg("a:" + this.value)};
        b.textContent = value;
        animButtons.appendChild(b);
    }
    if(topic == 'type') {
        if(value == 'battery') {
            try {document.getElementById('battery').style.display = "inline";}
            catch{console.log("no batt div found")}
            try{document.getElementById('sleep').style.display = "inline";}
            catch {}
        }
        if(value == 'wall') {
            try{document.getElementById('battery').style.display = "none"; }
            catch{}
            try{document.getElementById('sleep').style.display = "none";}
            catch{}
        }
    }
    if(topic == 't') {
        let timer = value.substring(0, value.indexOf(':'));
        value = value.substring(value.indexOf(':') + 1);
        if(timer == 'on') document.getElementById("onTime").value = value;
        if(timer == 'off')document.getElementById("offTime").value = value;
        if(timer == 'toggle') document.getElementById("timerSwitch").checked = (value == "1");
    }
    if(topic == 'print') {
        console.log(value);
    }
}
window.addEventListener('load', onLoad);

function sendMsg(msg) {
    websocket.send(msg);
}

function onLoad(event) {
    initWebSocket();
    //document.getElementById('animButtons').onpointerover = changeColor();
    //animButtons = document.getElementById("animations");
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
    setTimeout(() => { elem.style.color = 'white' }, 1000);
}

async function switchPage(page, button) {
    let p = document.getElementById("page");
    p.innerHTML = "<h1>Loading...</h1>";
    let response = await fetch("/"+page+".html");
    p.innerHTML = await response.text();
    let navs = document.getElementsByClassName("navButton");
    for(var i = 0; i < navs.length; i++) {
        navs[i].style.cssText = "background-color: #333;";
    }
    button.style.cssText = "background-color: red;"
    animButtons = document.getElementById("animations");
    sendMsg("update");
    animCount = 0;
    sendMsg('getAnimations');
}
/*<input type="button" value="Cylon" class="animButtons" onclick="sendMsg('a:cylon')">
            <input type="button" value="Cycle" class="animButtons" onclick="sendMsg('a:cycle')">
            <input type="button" value="Fall" class="animButtons" onclick="sendMsg('a:fall')">
            <input type="button" value="Halloween" class="animButtons" onclick="sendMsg('a:halloween')">*/