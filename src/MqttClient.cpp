#include <WiFi.h>
#include <PubSubClient.h>
#include "MqttClient.h"
#include "settings.hpp"

long LAST_RECONNECT_ATTEMPT = 0;

MqttClient::MqttClient(PubSubClient *pubSubClient) {
  this->client = pubSubClient;
  this->client->setServer(MQTT_HOST, atoi(MQTT_PORT));
}

bool MqttClient::sendMessage(String topic, String payload, bool retain) {
  return client->publish(topic.c_str(), payload.c_str(), retain);
}

bool MqttClient::sendMessage(String topic, String payload) {
  return this->sendMessage(topic, payload, false);
}

void MqttClient::keepAlive() {
  if (!this->client->connected()) {
    this->reconnect();
  } else {
    this->client->loop();
  }
}

bool MqttClient::reconnect() {
  int tries = 0;
  int maxTries = 12;

  while (!this->client->connected() && tries < maxTries) {
    DEBUG("Connecting to MQTT...");

    tries++;
    if (!this->client->connect(HOSTNAME, MQTT_USER, MQTT_PASS)) {
      delay(5000);
    }
  }

  if (tries >= maxTries) {
    return false;
  }

  return true;
}
