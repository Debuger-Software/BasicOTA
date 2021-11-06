#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#endif

#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#if defined(ESP32_RTOS) && defined(ESP32)
void ota_handle( void * parameter ) {
  for (;;) {
    ArduinoOTA.handle();
    delay(3500);
  }
}
#endif

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
    tone(BUZZER, 1400, 200);
    delay(100);
	//NOTE: make .detach() here for all functions called by Ticker.h library - not to interrupt transfer process in any way.
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    #ifdef REQUIRE_SD
    writeFileSD("/OTA.log", "OTA START : " + ArduinoOTA.getOTASenderIP().toString()+"\r\n");
    #endif
    Serial.println("╔═══════════════════════╦══════════════════════════╗");
    Serial.println("║ INCOMING UPDATE FROM  ║      "+ArduinoOTA.getOTASenderIP().toString()+"      ║");
    Serial.println("╠═══════════════════════╬══════════════════════════╣");
    tone(BUZZER, 2200, 300);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.print("\n\n\n");
    Serial.println("╠══════════════════════════════════════════════════╣");
    Serial.print("\r║\e[5m !WARNING! - FIRMWARE UPDATE COMPLETE - !WARNING!\e[0m ║\r\n");
    Serial.println("╠══════════════════════════════════════════════════╣");
    Serial.println("║      Press hardware reset button to reload.      ║");
    Serial.println("╚══════════════════════════════════════════════════╝\r\n\n");
    tone(BUZZER, 2000, 200);
    delay(250);
    tone(BUZZER, 2000, 200);
    delay(250);
    tone(BUZZER, 2000, 200);
    delay(250);
    Reboot();
    // for (;;);  ║  ╠   ╚═══╝      ╘══╛ │");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
     Serial.printf("║  Receiving firmware   ║   %06.2f KB / %06.2f KB  ║\r\n╠═══════════════════════╩══════════════════════════╣\r\n", (progress / 1024.00), (total / 1024.00));
    Serial.print("║");
    int _barprogress = progress / (total / 100);
    for (int p=1; p <= _barprogress / 2; p++) {Serial.print("█");}
    for (int q=_barprogress / 2; q < 50; q++) {Serial.print("░");}
    Serial.print("║\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");
    if (_barprogress > 50) Serial.print("\033[7m");
    Serial.printf("%u %%\033[0m\r\n╚══════════════════════════════════════════════════╝\r\e[A\e[A\e[A",_barprogress);
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

#if defined(ESP32_RTOS) && defined(ESP32)
  xTaskCreate(
    ota_handle,          /* Task function. */
    "OTA_HANDLE",        /* String with name of task. */
    10000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    NULL);            /* Task handle. */
#endif
}
