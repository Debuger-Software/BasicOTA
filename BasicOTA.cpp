#define LED_ON LOW
#define LED_OFF HIGH
#define TRANSMITER_1 0
const char* otaHostName = "DBGR-ESP8266-BLUE";

// #include <SPI.h>
#include "credentials.h"
#include "OTA.h"
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);



void handleRoot() {
  digitalWrite(TRANSMITER_1, HIGH);     delay(200);      digitalWrite(TRANSMITER_1, LOW);
  server.send(204);
}
void buttonHold_2s() {
  digitalWrite(TRANSMITER_1, HIGH);     delay(2000);     digitalWrite(TRANSMITER_1, LOW);
  server.send(204);
}
void buttonHold_5s() {
  digitalWrite(TRANSMITER_1, HIGH);     delay(5000);     digitalWrite(TRANSMITER_1, LOW);
  server.send(204);
}
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(TRANSMITER_1, OUTPUT);
  digitalWrite(TRANSMITER_1, LOW);  delay(250); digitalWrite(TRANSMITER_1, HIGH);  delay(200); digitalWrite(TRANSMITER_1, LOW);
  Serial.begin(115200);
  Serial.println("*********************** START BOOT ***********************");
  MDNS.begin(otaHostName);
  ArduinoOTA.setRebootOnSuccess(true);
  setupOTA(otaHostName, ssid, password);
  server.on("/light", handleRoot);
  server.on("/light/2", buttonHold_2s);
  server.on("/light/5", buttonHold_5s);
  server.begin();
  delay(100);
}

void loop() {
  ArduinoOTA.handle();
  if (!ArduinoOTA.getCommand()) { 
  digitalWrite(LED_BUILTIN, LED_ON);   // turn the LED on (HIGH is the voltage level)
  server.handleClient();
  delay(150); 
  digitalWrite(LED_BUILTIN, LED_OFF);    // turn the LED off by making the voltage LOW
  delay(200);                       // wait for a second
  }
}
