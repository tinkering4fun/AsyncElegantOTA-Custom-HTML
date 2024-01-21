 /*
  AsyncElegantOTA Demo Example - This example will work for both ESP8266 & ESP32 microcontrollers.
  -----
  Author: Ayush Sharma ( https://github.com/ayushsharma82 )
  
  Important Notice: Star the repository on Github if you like the library! :)
  Repository Link: https://github.com/ayushsharma82/AsyncElegantOTA

  
  tinkering4fun, January 2024
  ------
  Created fork https://github.com/tinkering4fun/AsyncElegantOTA-Custom-HTML
  This example to demonstrate the plain HTML/Javascript GUI
  The Demo runs in AP mode, and is accessible @ http://192.168.4.1
*/

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

// AP credentials, need no change for the demo
const char* ssid = "";
const char* password = "";

AsyncWebServer server(80);


void setup(void) {
  Serial.begin(115200);
  Serial.print(F("\n--- AsyncElegantOTA Demo ---\n"));
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Index page, containing a link to the tool
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/html", 
      R"htmlPage(
	<!DOCTYPE HTML>
	<html>
	  <head>
	    <title>AsyncElegantOTA Demo</title>
	    <meta name="viewport" content="width=device-width, initial-scale=1">
	  </head>
	  <body>
	    <p><b>AsyncElegantOTA </b>Demo</p>
	    <p>Click <a href="/update">here</a> to start the tool<p>
	  </body>
	</html>
      )htmlPage"
    );
  });

  // Start ElegantOTA, which is accessible at URL /update
  // Hint:
  // SIMPLE_HTML and SIMPLE_HTML_SIZE are defined in simpleWebpage.h
  // If required, take it as a template and adjust to your needs.
  //
  AsyncElegantOTA.begin(&server, "", "", SIMPLE_HTML, SIMPLE_HTML_SIZE, false);    
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  // This hook is required to trigger ESP.restart() after flashing
  AsyncElegantOTA.loop();
}
