#include "arduino_ota.h"
#include "settings.h"

void setupOTA() {
  ArduinoOTA.setPort(8266);
  ArduinoOTA.setHostname(HOSTNAME);
  ArduinoOTA.setPassword(OTA_PASSWORD);
  ArduinoOTA.begin();
}