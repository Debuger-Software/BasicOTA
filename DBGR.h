
void Reboot() {
  for (int x=1;x<20;x++) {
      digitalWrite(LED_BUILTIN, LED_ON);
      tone(0, 400, 75);
      delay(100);
      digitalWrite(LED_BUILTIN, LED_OFF);
      delay(100);
    }
    delay(100);
    ESP.restart();
}
