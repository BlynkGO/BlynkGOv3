#include <BlynkGOv3.h>
#include "src/BlynkGO_AsyncWebServer/BlynkGO_AsyncWebServer.h"

//--------------------------------------------------
// ออกแบบ หน้า HTML ตามต้องการในส่วนนี้
// ตัวอย่างจะมีช่อง input อยู่ 3 input fields (input1, input2, input3)
// เมื่อ User ใส่ค่า input มา ค่าจะนำมาแสดงบนจอ TFT

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    input1: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input2: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    input3: <input type="text" name="input3">
    <input type="submit" value="Submit">
  </form>
</body></html>
)rawliteral";

//--------------------------------------------------
// ชื่อ parameter ของ input บน html ข้างต้น
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

namespace AsyncWEBSERVER {
  AsyncWebServer server(80);

  void begin(){
    // Send web page with input fields to client
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
      request->send_P(200, "text/html", index_html);
    });
  
    // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
    server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
      String inputMessage;
      String inputParam;
      // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
      if (request->hasParam(PARAM_INPUT_1)) {
        inputMessage = request->getParam(PARAM_INPUT_1)->value();
        inputParam = PARAM_INPUT_1;
        lb_input1 = inputMessage;  // นำค่ามาแสดงบนวิตเจ็ตข้อความออกจอ TFT
      }
      // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
      else if (request->hasParam(PARAM_INPUT_2)) {
        inputMessage = request->getParam(PARAM_INPUT_2)->value();
        inputParam = PARAM_INPUT_2;
        lb_input2 = inputMessage; // นำค่ามาแสดงบนวิตเจ็ตข้อความออกจอ TFT
      }
      // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
      else if (request->hasParam(PARAM_INPUT_3)) {
        inputMessage = request->getParam(PARAM_INPUT_3)->value();
        inputParam = PARAM_INPUT_3;
        lb_input3 = inputMessage; // นำค่ามาแสดงบนวิตเจ็ตข้อความออกจอ TFT
      }
      else {
        inputMessage = "No message sent";
        inputParam = "none";
      }
      Serial.println(inputMessage);
      request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                       + inputParam + ") with value: " + inputMessage +
                                       "<br><a href=\"/\">Return to Home Page</a>");
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
