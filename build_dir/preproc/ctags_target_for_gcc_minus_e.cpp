# 1 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino"



const char* otaHostName = "DBGR-ESP8266-BLUE";

// #include <SPI.h>
# 8 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino" 2
# 9 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino" 2
# 10 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino" 2
# 11 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino" 2
ESP8266WebServer server(80);



void handleRoot() {
  digitalWrite(0, 0x0);
  delay(200);
  digitalWrite(0, 0x1);
  server.send(204);
}
void setup() {
  pinMode(2, 0x01);
  pinMode(0, 0x01);
  digitalWrite(0, 0x1); delay(200); digitalWrite(0, 0x0); delay(200); digitalWrite(0, 0x1);
  Serial.begin(115200);
  Serial.println("*********************** START BOOT ***********************");
  MDNS.begin(otaHostName);
  ArduinoOTA.setRebootOnSuccess(true);
  setupOTA(otaHostName, ssid, password);
  server.on("/light", handleRoot);
  server.begin();
  delay(100);
}

void loop() {
  ArduinoOTA.handle();
  if (!ArduinoOTA.getCommand()) {
  digitalWrite(2, 0x0); // turn the LED on (HIGH is the voltage level)
  server.handleClient();
  delay(150);
  digitalWrite(2, 0x1); // turn the LED off by making the voltage LOW
  delay(300); // wait for a second
  }
}
