#include <BlynkGOv3.h>
#include "src/BlynkGO_AsyncWebServer/BlynkGO_AsyncWebServer.h"

//--------------------------------------------------
// ออกแบบ หน้า HTML ตามต้องการในส่วนนี้
// ตัวอย่างจะมีช่อง input อยู่ 3 input fields (inputString, inputInt, inputFloat)
// เมื่อ User ใส่ค่า input มา ค่าจะนำมาแสดงบนจอ TFT

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script>
    function submitMessage() {
      alert("Saved value to ESP32 FlashMem");
      setTimeout(function(){ document.location.reload(false); }, 500);   
    }
  </script></head><body>
  <form action="/get" target="hidden-form">
    inputString (current value %inputString%): <input type="text" name="inputString">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <form action="/get" target="hidden-form">
    inputInt (current value %inputInt%): <input type="number " name="inputInt">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form><br>
  <form action="/get" target="hidden-form">
    inputFloat (current value %inputFloat%): <input type="number " name="inputFloat">
    <input type="submit" value="Submit" onclick="submitMessage()">
  </form>
  <iframe style="display:none" name="hidden-form"></iframe>
</body></html>
)rawliteral";

//--------------------------------------------------
// ชื่อ parameter ของ input บน html ข้างต้น
const char* PARAM_STRING = "inputString";
const char* PARAM_INT = "inputInt";
const char* PARAM_FLOAT = "inputFloat";

namespace AsyncWEBSERVER {
  AsyncWebServer server(80);
  // Replaces placeholder with stored values
  String processor(const String& var){
    //Serial.println(var);
    if(var == "inputString"){
      return BlynkGO.flashMem_String("INPUT_STR");   // ดึงข้อมูลจาก flashMem ออกมา ให้ WebServer
    }
    else if(var == "inputInt"){
      return BlynkGO.flashMem_String("INPUT_INT");   // ดึงข้อมูลจาก flashMem ออกมา ให้ WebServer
    }
    else if(var == "inputFloat"){
      return BlynkGO.flashMem_String("INPUT_FLOAT"); // ดึงข้อมูลจาก flashMem ออกมา ให้ WebServer
    }
    return String();
  }
  
  void begin(){
    // Send web page with input fields to client
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html, AsyncWEBSERVER::processor);
    });
  
    // Send a GET request to <ESP_IP>/get?inputString=<inputMessage>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String inputMessage;
      // GET inputString value on <ESP_IP>/get?inputString=<inputMessage>
      if (request->hasParam(PARAM_STRING)) {
        inputMessage = request->getParam(PARAM_STRING)->value();
        BlynkGO.flashMem( "INPUT_STR", inputMessage);   // บันทึกลง flashMem
        lb_input_str = inputMessage;
      }
      // GET inputInt value on <ESP_IP>/get?inputInt=<inputMessage>
      else if (request->hasParam(PARAM_INT)) {
        inputMessage = request->getParam(PARAM_INT)->value();
        BlynkGO.flashMem( "INPUT_INT", inputMessage);   // บันทึกลง flashMem
        lb_input_int = inputMessage;
      }
      // GET inputFloat value on <ESP_IP>/get?inputFloat=<inputMessage>
      else if (request->hasParam(PARAM_FLOAT)) {
        inputMessage = request->getParam(PARAM_FLOAT)->value();
        BlynkGO.flashMem( "INPUT_FLOAT", inputMessage);   // บันทึกลง flashMem
        lb_input_float = inputMessage;
      }
      else {
        inputMessage = "No message sent";
      }
      Serial.println(inputMessage);
      request->send(200, "text/text", inputMessage);
    });
    
    server.onNotFound([](AsyncWebServerRequest *request) {
      request->send(404, "text/plain", "Not found");
    });
    server.begin();
  }
  void end(){
    server.end();
  }
}


