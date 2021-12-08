#line 1 "c:\\Users\\dymar\\Desktop\\BasicOTA\\OTA.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>

#include <WiFiUdp.h>
#include <ArduinoOTA.h>


void setupOTA(const char* nameprefix, const char* ssid, const char* password) {
  ArduinoOTA.setHostname(nameprefix);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    Reboot();
  }
  // ArduinoOTA.setPort(3232); // Use 8266 port if you are working in Sloeber IDE, it is fixed there and not adjustable
  // ArduinoOTA.setPassword("admin");
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  ArduinoOTA.onStart([]() {
    tone(1400, 200);
    delay(100);
	//NOTE: make .detach() here for all functions called by Ticker.h library - not to interrupt transfer process in any way.
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
  });
  
  ArduinoOTA.onEnd([]() {

  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {

  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("\nAuth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("\nBegin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("\nConnect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("\nReceive Failed");
    else if (error == OTA_END_ERROR) Serial.println("\nEnd Failed");
  });

  ArduinoOTA.begin();

  Serial.print("\e[36m[D]:OTA Service initialized    ----     ");
  Serial.print(WiFi.localIP());
  Serial.print(':');
  Serial.println("\e[36m"+String(ArduinoOTA.getOTAPort())+" \e[0m ");

}
