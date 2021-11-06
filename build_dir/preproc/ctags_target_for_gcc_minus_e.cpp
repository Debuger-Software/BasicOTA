# 1 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino"
//#define REQUIRE_SD
# 12 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino"
const char* otaHostName = "DBGR-ESP8266-BLUE";


# 16 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino" 2



int last_millis = millis();
int min_millis = 999999;
int max_millis, last_diff;





# 28 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino" 2
# 29 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino" 2
# 30 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino" 2




# 35 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino" 2
ESP8266WebServer server(80);




void handleRoot() {
  digitalWrite(0, 0x0);
  delay(200);
  digitalWrite(0, 0x1);
  server.send(204);



}
void setup() {
  pinMode(2, 0x01);



  pinMode(4, 0x01);

  pinMode(0, 0x01);
  digitalWrite(0, 0x1);
  Serial.begin(115200);
  Serial.println("\n\n");
  Serial.printf("************************ %u ms *************************\r\n", last_millis);
  Serial.println("*********************** START BOOT ***********************");
# 70 "c:\\Users\\dymar\\Desktop\\BasicOTA\\BasicOTA.ino"
  MDNS.begin(otaHostName);
  ArduinoOTA.setRebootOnSuccess(true);
  setupOTA(otaHostName, ssid, password);
  server.on("/light", handleRoot);
  server.begin();
  Serial.println("\e[36m[D]:HTTP server started\e[0m");
  Serial.println("********************** BOOT COMPLETE *********************");
  Serial.printf("************************ + %u ms ***********************\r\n\n", (millis() - last_millis));
  last_millis = millis();
  tone(4, 1400, 200);
  delay(100);
}

void loop() {
  ArduinoOTA.handle();
  if (!ArduinoOTA.getCommand()) {
  digitalWrite(2, 0x0); // turn the LED on (HIGH is the voltage level)
  // digitalWrite(TRANSMITER_1, !digitalRead(TRANSMITER_1));
  server.handleClient();
  last_diff = millis() - last_millis;
  last_millis = millis();

  if (last_diff < min_millis && last_diff > 1000) {min_millis = last_diff;}
  else if (last_diff > max_millis && last_diff < 4000) {max_millis = last_diff;}
  Serial.printf(" + %u ms    (min: %u , max: %u , diff: %u)                              \r", last_diff, min_millis, max_millis, (max_millis - min_millis));
                      // wait for a second
  delay(100);
  digitalWrite(2, 0x1); // turn the LED off by making the voltage LOW
  delay(900); // wait for a second
  }
}
