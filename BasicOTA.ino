//#define REQUIRE_SD

#ifdef ESP32
#define ESP32_RTOS                // Uncomment this line if you want to use the code with freertos only on the ESP32
#define LED_BUILTIN 2
#define LED_ON HIGH
#define LED_OFF LOW
const char* otaHostName = "DBGR-ESP32";
#else
#define LED_ON LOW
#define LED_OFF HIGH
const char* otaHostName = "DBGR-ESP8266-BLUE";
#endif

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>

#define BUZZER 4
#define TRANSMITER_1 0
int last_millis = millis();
int min_millis = 999999;
int max_millis, last_diff;

#ifdef REQUIRE_SD
const int chipSelect = 5;
#include <SD.h>
#endif
#include "DBGR.h"
#include "credentials.h"
#include "OTA.h"
#ifdef ESP32
#include <WebServer.h>
WebServer server(80);
#else
#include <ESP8266WebServer.h>
ESP8266WebServer server(80);
#endif
LiquidCrystal_I2C lcd(0x27,20,4);



void handleRoot() {
  digitalWrite(TRANSMITER_1, LOW);
  delay(200);
  digitalWrite(TRANSMITER_1, HIGH);
  server.send(204);
  #ifdef REQUIRE_SD
  writeFileSD("/SERVER.log", "SIGNAL\r\n");
  #endif
}
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,2);
  pinMode(LED_BUILTIN, OUTPUT);
  #ifdef ESP32
  ledcAttachPin(BUZZER, 0);
  #else
  pinMode(BUZZER, OUTPUT);
  #endif
  pinMode(TRANSMITER_1, OUTPUT);
  digitalWrite(TRANSMITER_1, HIGH);
  Serial.begin(115200);
  Serial.println("\n\n");
  Serial.printf("************************ %u ms *************************\r\n", last_millis);
  Serial.println("*********************** START BOOT ***********************");
  #ifdef REQUIRE_SD
  initSD();
  if (!SD.exists("/OTA.log")) {Serial.println("\e[36m[D]:--Warning: OTA.log not exist--\e[0m");}
  else {
    File root = SD.open("/");
    printDirectory(root, 0);
  }
  #endif
  MDNS.begin(otaHostName);
  ArduinoOTA.setRebootOnSuccess(true);
  setupOTA(otaHostName, ssid, password);
  server.on("/light", handleRoot);
  server.begin();
  Serial.println("\e[36m[D]:HTTP server started\e[0m");
  Serial.println("********************** BOOT COMPLETE *********************");
  Serial.printf("************************ + %u ms ***********************\r\n\n", (millis() - last_millis));
  lcd.print("-- BOOT  COMPLETE --");
  lcd.setCursor(0,3);
  lcd.printf("--    %u ms    --",millis() - last_millis);
  tone(BUZZER, 1400, 200);
  delay(100);
  last_millis = millis();
}

void loop() {
  ArduinoOTA.handle();
  if (!ArduinoOTA.getCommand()) { 
  digitalWrite(LED_BUILTIN, LED_ON);   // turn the LED on (HIGH is the voltage level)
  // digitalWrite(TRANSMITER_1, !digitalRead(TRANSMITER_1));
  server.handleClient();
  last_diff = millis() - last_millis;
  last_millis = millis();
  
  if (last_diff < min_millis && last_diff > 1000) {min_millis = last_diff;}
  else if (last_diff > max_millis && last_diff < 4000) {max_millis = last_diff;}
  Serial.printf(" + %u ms    (min: %u , max: %u , diff: %u)                              \r", last_diff, min_millis, max_millis, (max_millis - min_millis));
                      // wait for a second
  delay(100); 
  digitalWrite(LED_BUILTIN, LED_OFF);    // turn the LED off by making the voltage LOW
  delay(900);                       // wait for a second
  }
}
