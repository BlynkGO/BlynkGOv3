#include <BlynkGOv3.h>
#include "src/BlynkGO_AsyncWebServer/BlynkGO_AsyncWebServer.h"

//--------------------------------------------------
// ออกแบบ หน้า HTML ตามต้องการในส่วนนี้

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
   }
   /*.button:hover {background-color: #0f8b8d}*/
   .button:active {
     background-color: #0f8b8d;
     box-shadow: 2 2px #CDCDCD;
     transform: translateY(2px);
   }
   .state {
     font-size: 1.5rem;
     color:#8c8c8c;
     font-weight: bold;
   }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Output - GPIO 25</h2>
      <p class="state">state: <span id="state">%STATE%</span></p> <!-- %STATE% ตัวแปรที่จะแสดงตาม ws.textAll(..) ผ่าน onMessage ของ javscript -->
      <p><button id="button" class="button">Toggle</button></p>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway); // เริ่มทำงาน websocket
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line จุดรับข้อมูลจากภายนอก ws.textAll(..) เข้ามา
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {  // จุดรับ ข้อมูลจาก ws.textAll(..) ที่ส่งเข้ามา
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state; // นำข้อมูลจากภายนอกเข้ามากำหนดให้ html
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    websocket.send('toggle'); // ส่งคำสั่ง 'toggle' ไปยัง websocket ภายนอก --> ไปยัง data(..) ของ handleWebSocketMessage
  }
</script>
</body>
</html>
)rawliteral";
//--------------------------------


namespace AsyncWEBSOCKET {
  AsyncWebServer server(80);
  AsyncWebSocket ws("/ws");

  void notifyClients(bool sw_state){
    ws.textAll(String(sw_state));  // ส่งข้อมูลไปให้ html javascript ที่ onMessage ของ websokcetใน javascript
  }
  
  void onData(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
      data[len] = 0;
      if(String((char*)data)== "toggle"){
        sw3d.toggle();
        AsyncWEBSOCKET::notifyClients(sw3d.isON()); // ส่งสถานะ sw3d เข้าไปยัง websokcet ของ html ผ่าน ws.textAll(..)
      }
    }
  }
  
  String processor(const String& var){
    Serial.println(var);
    if(var == "STATE"){
      return sw3d.isON()? "ON" : "OFF"; // แสดงตอนเริ่มต้นเท่านั้น
    }
    return String();
  }
  
  void begin() {
    ws.onEvent([](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType event,
               void *arg, uint8_t *data, size_t len) {
      switch (event) {
        case WS_EVT_CONNECT:
          Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
          break;
        case WS_EVT_DISCONNECT:
          Serial.printf("WebSocket client #%u disconnected\n", client->id());
          break;
        case WS_EVT_DATA:
          AsyncWEBSOCKET::onData(arg, data, len);
          break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
          break;
      }
    });
    server.addHandler(&ws);
    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html, AsyncWEBSOCKET::processor);
    });
  
    // Start server
    server.begin();
  }

  void end(){
    server.end();
  }
  

  void update(){
    ws.cleanupClients();
  }
} // namespace AsyncWEBSOCKET

