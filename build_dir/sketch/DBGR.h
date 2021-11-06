#line 1 "c:\\Users\\dymar\\OneDrive\\Pulpit\\BasicOTA\\DBGR.h"
#ifdef ESP32
void tone(double bpin, double bfreq, double btms = 0) {
  ledcWriteTone(bpin, bfreq);
  if (btms >= 1) {
    delay(btms);
    ledcWriteTone(bpin, 0);
  }
}
#endif

void Reboot() {
  for (int x=1;x<20;x++) {
      digitalWrite(LED_BUILTIN, LED_ON);
      tone(BUZZER, 400, 75);
      delay(100);
      digitalWrite(LED_BUILTIN, LED_OFF);
      delay(100);
    }
    delay(100);
    ESP.restart();
}

#ifdef REQUIRE_SD
void initSD() {
  Serial.print("\e[36m[D]:Initializing SD card...");
  if (!SD.begin(5)) {
    Serial.println("initialization failed.\e[0m ");
    Reboot();
  } else { Serial.println("OK. Memory detected.\e[0m "); }
}

void writeFileSD(String path, String dataString) {
  File dataFile = SD.open(path, FILE_WRITE);
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("\e[36m[D]:[SD]["+path+"][WRITE]:"+dataString+"\e[0m ");
  } else { Serial.println("error opening datalog.txt"); }
}

void printDirectory(File dir, int numTabs) {
  for (uint8_t i = 0; i < numTabs; i++) {Serial.print('\t');}
  Serial.println("   FILENAME           SIZE   \t    CREATE    \t    MODIFY");
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) break;
    for (uint8_t i = 0; i < numTabs; i++) {Serial.print("  |---- ");}
    if (entry.isDirectory()) {
      Serial.print('[');
      Serial.print(entry.name());
      Serial.print("]\r\n");
      printDirectory(entry, numTabs + 1);
    } else {
      Serial.print(entry.name());
      for (uint8_t i = 0; i < 20 - String(entry.name()).length(); i++) {Serial.print(' ');}
      Serial.printf("%.2f KB", entry.size() / 1024.00);
      time_t cr = entry.getCreationTime();
      time_t lw = entry.getLastWrite();
      struct tm * tmstruct = localtime(&cr);
      Serial.printf("\t%02d/%02d %02d:%02d:%02d", (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
      tmstruct = localtime(&lw);
      Serial.printf("\t%02d/%02d %02d:%02d:%02d\r\n", (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
    }
    entry.close();
  }
}

#endif