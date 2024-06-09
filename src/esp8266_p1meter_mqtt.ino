#include "settings.hpp"
#include <WiFi.h>
#include <dsmr.h>
#include "MqttClient.h"
#include "arduino_ota.h"

WiFiClient espClient;
PubSubClient pubSubClient(espClient);
MqttClient mqttClient(&pubSubClient);
P1Reader reader(&Serial2, 2);
unsigned long lastKeepAlive;

struct MqttPublisher {
  template <typename Item> void apply(Item &i) {
    if (i.present()) {
      String topic = String(MQTT_ROOT_TOPIC) + "/" + String(Item::name);
      // String value = String(i.val()) + String(Item::unit());
      String value = String(i.val());
      DEBUG(String("Sending " + topic + " = " + value));

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
  Serial2.begin(BAUD_RATE, SERIAL_8N1, RXD2, TXD2, true);

  DEBUG("Starting Wifi");
  wifiKeepAlive();

  DEBUG("Setup OTA");
  setupOTA();

  DEBUG("Connecting to MQTT");
  mqttClient.keepAlive();
  mqttClient.sendMessage(String(MQTT_ROOT_TOPIC) + String("/healthcheck"), "connected");

  DEBUG("Setup reader");
  reader.enable(false);

  MyData data;

  DEBUG("Autodiscovery");
  data.applyEach(AutoDiscoveryPublisher());
}

void loop() {
  ArduinoOTA.handle();

  // No need to keep alive every loop, that's much too often and will result in some P1 messages
  // being lost, so just be chill and do it every 5 seconds
  long now = millis();
  if (now - lastKeepAlive > 5000) {
    wifiKeepAlive();
    mqttClient.keepAlive();
    lastKeepAlive = now;

    DEBUG("Healthcheck");
    mqttClient.sendMessage(String(MQTT_ROOT_TOPIC) + String("/healthcheck"), "alive");
  }

  if (reader.loop()) {
    MyData data;
    String err;
    if (reader.parse(&data, &err)) {
      // Parse succesful, send to mqtt
      data.applyEach(MqttPublisher());
    } else {
      // Parser error, print error
      DEBUG(err);
    }
  }
}

void wifiKeepAlive() {
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) { 
      delay(5000);
      ESP.restart();
    }
  }
}