#define LED_ON LOW
#define LED_OFF HIGH
#define TRANSMITER_1 0
const char* otaHostName = "DBGR-ESP8266-BLUE";

#include "credentials.h"
#include "OTA.h"
#include <ESP8266WebServer.h>
#include <uri/UriBraces.h>
#include <uri/UriRegex.h>
ESP8266WebServer server(80);



void handleRoot() {
  digitalWrite(TRANSMITER_1, HIGH);     delay(200);      digitalWrite(TRANSMITER_1, LOW);
  server.send(204);
}
void buttonHold() {
  if (server.pathArg(0).toInt() < 1 || server.pathArg(0).toInt() > 10) {server.send(500); return;}
  digitalWrite(TRANSMITER_1, HIGH);     delay(server.pathArg(0).toInt()*1000);     digitalWrite(TRANSMITER_1, LOW);
  server.send(204);
}
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRANSMITER_1, OUTPUT);
  digitalWrite(TRANSMITER_1, LOW);  delay(250); digitalWrite(TRANSMITER_1, HIGH);  delay(200); digitalWrite(TRANSMITER_1, LOW);
  MDNS.begin(otaHostName);
  ArduinoOTA.setRebootOnSuccess(true);
  setupOTA(otaHostName, ssid, password);
  server.on("/light", handleRoot);
  server.on(UriBraces("/light/{}"), buttonHold);
  server.begin();
  delay(100);
}

void loop() {
  ArduinoOTA.handle();
  if (!ArduinoOTA.getCommand()) { 
  digitalWrite(LED_BUILTIN, LED_ON); 
  server.handleClient();
  delay(100); 
  digitalWrite(LED_BUILTIN, LED_OFF);
  delay(250);
  }
}
