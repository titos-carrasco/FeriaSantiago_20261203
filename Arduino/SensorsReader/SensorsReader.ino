#include <ESP8266WiFi.h>
WiFiClient wifi;

#include <ArduinoJson.h>
JsonDocument jsonDoc;
DeserializationError jsonError;

#include <MQTT.h>
MQTTClient mqtt(1024);

char dht22_temp[] = "+.....";
char dht22_hume[] = "+.....";
char dht22_volt[] = "+.....";
char dht22_date[] = "+01/01/1970 00:00:00";

char mpl3115a2_temp[] = "+......";
char mpl3115a2_pres[] = "+......";
char mpl3115a2_alti[] = "+......";
char mpl3115a2_volt[] = "+......";
char mpl3115a2_date[] = "+01/01/1970 00:00:00";

char novapm_25[] = "+......";
char novapm_10[] = "+......";
char novapm_volt[] = "+......";
char novapm_date[] = "+01/01/1970 00:00:00";

#include "MiConfig.h"

char line[40];
int linePos = 0;

// inicializacion del programa
void setup() {
  // puerta serial
  Serial.begin(300);
  Serial.flush();

  // wifi
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // mqtt
  mqtt.begin(MQTT_SERVER, MQTT_PORT, wifi);
  mqtt.onMessage(messageReceived);
  mqtt.loop();
  delay(10);
}

// ciclo de ejecucion
void loop() {
  // el comando a ejecutar
  doCmd();

  // la conexion wifi
  if (WiFi.status() != WL_CONNECTED) return;

  // la conexion mqtt
  if (!mqtt.connected()) {
    if (mqtt.connect(MQTT_CLIENT_ID, MQTT_CLIENT_USER, MQTT_CLIENT_PASS, false)) {
      mqtt.subscribe(MQTT_TOPIC_MPL3115A2);
      mqtt.subscribe(MQTT_TOPIC_DHT22);
      mqtt.subscribe(MQTT_TOPIC_NOVAPM);
    }
  }

  mqtt.loop();
  delay(10);
}

void doCmd() {
  int b = Serial.read();
  if (b < 0) return;
  if (b != '\r') {
    line[linePos++] = b;
    if (linePos == sizeof(line))
      linePos = 0;
    return;
  }
  line[linePos] = 0;
  linePos = 0;

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("-WIFI NO CONECTADA\r");
    Serial.flush();
    return;
  }

  if (!mqtt.connected()) {
    Serial.print("-MQTT NO CONECTADO\r");
    Serial.flush();
    return;
  }

  if (strcmp(line, "STAT") == 0) {
    Serial.print("+READY\r");
    Serial.flush();
    return;
  }

  if (strcmp(line, "TEMP") == 0) {
    Serial.print("+TEMP");
    Serial.print(dht22_temp);
    Serial.print(dht22_hume);
    Serial.print(dht22_volt);
    Serial.print(dht22_date);
    Serial.write('\r');
    Serial.flush();
    return;
  }

  if (strcmp(line, "PRES") == 0) {
    Serial.print("+PRES");
    Serial.print(mpl3115a2_temp);
    Serial.print(mpl3115a2_pres);
    Serial.print(mpl3115a2_alti);
    Serial.print(mpl3115a2_volt);
    Serial.print(mpl3115a2_date);
    Serial.write('\r');
    Serial.flush();
    return;
  }

  if (strcmp(line, "NOVA") == 0) {
    Serial.print("+NOVA");
    Serial.print(novapm_25);
    Serial.print(novapm_10);
    Serial.print(novapm_volt);
    Serial.print(novapm_date);
    Serial.write('\r');
    Serial.flush();
    return;
  }

  Serial.print("-COMANDO INVALIDO\r");
  Serial.flush();
}

void messageReceived(String& topic, String& payload) {
  jsonError = deserializeJson(jsonDoc, payload);
  if (jsonError) return;

  if (topic.equals(MQTT_TOPIC_DHT22)) {
    sprintf(dht22_temp+1, "%5.1f", jsonDoc["temperatura"].as<float>());
    sprintf(dht22_hume+1, "%5.1f", jsonDoc["humedad"].as<float>());
    sprintf(dht22_volt+1, "%5.1f", jsonDoc["voltaje"].as<float>());
    sprintf(dht22_date+1, "%19.19s", jsonDoc["datetime"].as<const char*>());
    return;
  }

  if (topic.equals(MQTT_TOPIC_MPL3115A2)) {
    sprintf(mpl3115a2_temp+1, "%6.1f", jsonDoc["temperatura"].as<float>());
    sprintf(mpl3115a2_pres+1, "%6.1f", jsonDoc["presion"].as<float>());
    sprintf(mpl3115a2_alti+1, "%6.1f", jsonDoc["altitud"].as<float>());
    sprintf(mpl3115a2_volt+1, "%6.1f", jsonDoc["voltaje"].as<float>());
    sprintf(mpl3115a2_date+1, "%19.19s", jsonDoc["datetime"].as<const char*>());
    return;
  }

  if (topic.equals(MQTT_TOPIC_NOVAPM)) {
    sprintf(novapm_25+1, "%6.1f", jsonDoc["pm25"].as<float>());
    sprintf(novapm_10+1, "%6.1f", jsonDoc["pm10"].as<float>());
    sprintf(novapm_volt+1, "%6.1f", jsonDoc["voltaje"].as<float>());
    sprintf(novapm_date+1, "%19.19s", jsonDoc["datetime"].as<const char*>());
    return;
  }
}
