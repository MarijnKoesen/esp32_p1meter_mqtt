#include "settings.h"
#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h>
#include <dsmr.h>
#include "MqttClient.h"
#include "arduino_ota.h"

WiFiClient espClient;
MqttClient mqttClient(espClient);
P1Reader reader(&Serial, 2);

struct MqttPublisher {
  template <typename Item> void apply(Item &i) {
    if (i.present()) {
      String topic = String(MQTT_ROOT_TOPIC) + "/" + String(Item::name);
      // String value = String(i.val()) + String(Item::unit());
      String value = String(i.val());

      mqttClient.sendMessage(topic, value);
    }
  }
};

struct AutoDiscoveryPublisher {
  template <typename Item> void apply(Item &i) {
    String topic = String(MQTT_AUTODISCOVERY_TOPIC) + String("/") + String(Item::name) + String("/config");
    String value = String(
      "{"
        "\"name\": \"P1 ") + String(Item::name) + String("\","
        "\"unique_id\": \"") + String(MQTT_DEVICE_ID) + String("_") + String(Item::name) + String("\","
        "\"unit_of_measurement\": \"") + String(Item::unit()) + String("\","
        "\"state_topic\": \"") + String(MQTT_ROOT_TOPIC) + String("/") + String(Item::name) + String("\","
        "\"force_update\": true,"
        "\"device\": {"
          "\"identifiers\": ["
            "\"") + String(MQTT_DEVICE_ID) + String("\""
          "],"
          "\"name\": \"P1 Power Meter\""
        "}"
      "}"
      );

    // Retain these messages, because we don't want to loose our devices
    mqttClient.sendMessage(topic, value, true);
  }
};

void setup() {
  Serial.begin(BAUD_RATE);
  wifiKeepAlive();
  mqttClient.keepAlive();

  setupOTA();
  reader.enable(false);

  MyData data;
  data.applyEach(AutoDiscoveryPublisher());
}

void loop() {
  wifiKeepAlive();
  mqttClient.keepAlive();
  ArduinoOTA.handle();

  if (reader.loop()) {
    MyData data;
    String err;
    if (reader.parse(&data, &err)) {
      // Parse succesful, send to mqtt
      data.applyEach(MqttPublisher());
    } else {
      // Parser error, print error
      // Serial.println(err);
    }
  }
}

void wifiKeepAlive() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
    }
  }
}