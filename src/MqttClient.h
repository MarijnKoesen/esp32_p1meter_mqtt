#ifndef MQTT_H
#define MQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MqttClient {
public:
  MqttClient(WiFiClient &espClient);
  void keepAlive();
  // bool sendMessage(const char *topic, char *payload);
  bool sendMessage(String topic, String payload);
  bool sendMessage(String topic, String payload, boolean retrain);

private:
  PubSubClient client;

  bool reconnect();
};

#endif