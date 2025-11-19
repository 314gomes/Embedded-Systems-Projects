#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

// FreeRTOS includes
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

const char* host = "esp32";
const char* ssid = "LabMicros";
const char* password = "seluspeesc@";

WebServer server(80);

// HTML content for the login page
const char* loginPageContent = 
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='A09F9F' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font size=4><b>ESP32 Login Page</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<td>Username:</td>"
        "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/updateFirmware')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')"
    "}"
    "}"
"</script>";
 
// HTML content for the firmware update page
const char* updatePageContent = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

// Global definitions for LED and blink delay
const int LED_BUILTIN_PIN = LED_BUILTIN; // Use the predefined LED_BUILTIN macro for the onboard LED (often GPIO2)
const int BLINK_DELAY_MS = 1000; // 1 second blink delay

// Task to handle LED blinking
void LedBlinkTask(void *pvParameters) {
  // Ensure the LED pin is set as output
  pinMode(LED_BUILTIN_PIN, OUTPUT);
  Serial.println("LED Blink Task started.");

  for (;;) {
    digitalWrite(LED_BUILTIN_PIN, HIGH); // Turn the LED on
    vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS)); // Wait for BLINK_DELAY_MS
    digitalWrite(LED_BUILTIN_PIN, LOW);  // Turn the LED off
    vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS)); // Wait for BLINK_DELAY_MS
  }
}

// Task to handle Web Server client requests
void WebServerTask(void *pvParameters) {
  Serial.println("Web Server Task started.");
  for (;;) {
    server.handleClient(); // Handle incoming client requests
    vTaskDelay(1); // Yield to other tasks to prevent watchdog timer issues
  }
}

void setup(void) {
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up mDNS for hostname resolution (e.g., http://esp32.local)
  if (!MDNS.begin(host)) { 
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  
  // Route for the root URL, serves the login page
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginPageContent);
  });
  // Route for the firmware update page, accessible after login
  server.on("/updateFirmware", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", updatePageContent);
  });
  // Route to handle firmware uploads (POST request)
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      // Start of upload, begin the update process
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { 
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      // Writing received data to flash
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      // End of upload, finalize the update
      if (Update.end(true)) { 
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin(); // Start the web server
  Serial.println("HTTP server started.");

  // Create FreeRTOS tasks
  // xTaskCreate( TaskFunction, Name, StackDepth, Parameters, Priority, Handle );
  xTaskCreate(
    LedBlinkTask,     // Task function
    "LedBlinkTask",   // Name of task
    1024,             // Stack size (words, 1024 * 4 bytes = 4KB)
    NULL,             // Parameter to pass to the function
    1,                // Task priority (0 is the lowest, configMAX_PRIORITIES-1 is the highest)
    NULL              // Task handle
  );

  xTaskCreate(
    WebServerTask,    // Task function
    "WebServerTask",  // Name of task
    4096,             // Stack size (words, 4096 * 4 bytes = 16KB - increased for web server operations)
    NULL,             // Parameter to pass to the function
    2,                // Task priority (higher than LED task for responsiveness)
    NULL              // Task handle
  );

  Serial.println("FreeRTOS tasks created.");
}

void loop(void) {
  // FreeRTOS tasks handle all continuous operations, so loop() can be empty.
  // The Arduino loop task will essentially run idle or yield to other tasks.
}