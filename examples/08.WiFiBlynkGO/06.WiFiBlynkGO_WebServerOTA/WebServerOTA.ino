#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

 // หลังตั้ง DSN Server (Bonjour Server) จะเรียก esp32.local แทน IP Address ได้
#define ESP32_HOST      "esp32"
WebServer ota_server(80);

// Login Page 
const char* login_html = R"(
<form name='loginForm'>
    <table width='20%' bgcolor='A09F9F' align='center'>
        <tr>
            <td colspan=2>
                <center><font size=4><b>ESP32 Login Page</b></font></center>
                <br>
            </td>
            <br>
            <br>
        </tr>
        <tr>
             <td>Username:</td>
             <td><input type='text' size=25 name='userid'><br></td>
        </tr>
        <br>
        <br>
        <tr>
            <td>Password:</td>
            <td><input type='Password' size=25 name='pwd'><br></td>
            <br>
            <br>
        </tr>
        <tr>
            <td><input type='submit' onclick='check(this.form)' value='Login'></td>
        </tr>
    </table>
</form>
<script>
    function check(form)
    {
    if(form.userid.value=='admin' && form.pwd.value=='admin')
    {
    window.open('/serverIndex')
    }
    else
    {
     alert('Error Password or Username')/*displays error message*/
    }
    }
</script>
)";

// WebServerOTA Upload Page 
const char* WebServerOTA_html = R"(
<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>
<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>
    <input type='file' name='update'>
    <input type='submit' value='Update'>
</form>
<div id='prg'>progress: 0%</div>
<script>
    $('form').submit(function(e){
        e.preventDefault();
        var form = $('#upload_form')[0];
        var data = new FormData(form);
        $.ajax({
            url: '/update',
            type: 'POST',
            data: data,
            contentType: false,
            processData:false,
            xhr: function() {
                var xhr = new window.XMLHttpRequest();
                xhr.upload.addEventListener('progress', function(evt) {
                    if (evt.lengthComputable) {
                        var per = evt.loaded / evt.total;
                        $('#prg').html('progress: ' + Math.round(per*100) + '%');
                    }
                }, false);
                return xhr;
            },
            success:function(d, s) {
                console.log('success!')
            },
            error: function (a, b, c) {
            }
        });
    });
</script>
)";


void WebServerOTA::begin(){
  /*use mdns for host name resolution*/
  if (!MDNS.begin(ESP32_HOST)) { //http://esp32.local
    Serial.println("[mDNS] Error setting up mDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  MDNS.addService("http", "tcp", 80);
  Serial.println("[mDNS] mDNS responder started");
  
  /*return index page which is stored in serverIndex */
  ota_server.on("/", HTTP_GET, []() {
    ota_server.sendHeader("Connection", "close");
    ota_server.send(200, "text/html", login_html);
  });
  ota_server.on("/serverIndex", HTTP_GET, []() {
    ota_server.sendHeader("Connection", "close");
    ota_server.send(200, "text/html", WebServerOTA_html);
  });
  /*handling uploading firmware file */
  ota_server.on("/update", HTTP_POST, []() {
    ota_server.sendHeader("Connection", "close");
    ota_server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = ota_server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  
  Serial.println("[WebServerOTA] begin");
  ota_server.begin();
}

void WebServerOTA::handle(){
  ota_server.handleClient();
}
