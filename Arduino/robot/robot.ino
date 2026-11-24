#include <ESP8266WiFi.h>
WiFiClient wifi;

#include <time.h>
const char* TZ_INFO = "CLT4CLST,M9.1.6/24,M4.1.6/24";

#include <ArduinoJson.h>
JsonDocument jsonDoc;
DeserializationError jsonError;
char buffer[512];

#include <MQTT.h>
MQTTClient mqtt;

#include "MiConfig.h"
int cmd = 0;

void setup() {
  // inicia monitor serial
  debug_begin(115200);
  debugln();
  debugln();
  debug_flush();

  // inicializa la red
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // inicializa ntp
  configTime(TZ_INFO, "pool.ntp.org", "time.nist.gov");

  // inicializa mqtt
  mqtt.begin(MQTT_SERVER, MQTT_PORT, wifi);
  mqtt.onMessage(messageReceived);
  mqtt.loop();
  delay(10);

  // inicializa motores
  pinMode(MOTOR_PWMA, OUTPUT);
  pinMode(MOTOR_DIRA, OUTPUT);
  pinMode(MOTOR_PWMB, OUTPUT);
  pinMode(MOTOR_DIRB, OUTPUT);
  motor(0, 0);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    debugln("WiFi no conectada. Reintentando ...");
    debug_flush();
    delay(2000);
    return;
  }

  if (mqtt.connected()) {
    switch (cmd) {
      case 0:
        {
          break;
        }
      case CMD_FORWARD:
        {
          debugln("Forward");
          debug_flush();
          motor(255, 255);
          break;
        }
      case CMD_BACKWARD:
        {
          debugln("Backward");
          debug_flush();
          motor(-255, -255);
          break;
        }
      case CMD_LEFT:
        {
          debugln("Left");
          debug_flush();
          motor(0, 255);
          break;
        }
      case CMD_RIGHT:
        {
          debugln("Right");
          debug_flush();
          motor(255, 0);
          break;
        }
      case CMD_INFO:
        {
          debugln("Info");
          debug_flush();
          jsonDoc.clear();
          jsonDoc["nodo_id"] = NODO_ID;
          jsonDoc["ip"] = WiFi.localIP().toString();

          time_t now = time(nullptr);
          struct tm* timeinfo = localtime(&now);
          char timeBuffer[25];
          strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", timeinfo);
          jsonDoc["datetime"] = timeBuffer;

          yield();

          serializeJson(jsonDoc, buffer);
          debug("Info: ");
          debug(buffer);
          debugln();
          debug_flush();

          mqtt.publish(MQTT_TOPIC_ROBOT_INFO, buffer);
          break;
        }
      case CMD_STOP:
        {
          debugln("Stop");
          debug_flush();
          motor(0, 0);
          break;
        }
      default:
        {
          debugln("Desconocido");
          debug_flush();
          motor(0, 0);
          break;
        }
    }
    cmd = 0;
    mqtt.loop();
    delay(10);

  } else {
    debugln("MQTT no conectado. Reintentando ...");
    debug_flush();
    if (mqtt.connect(MQTT_CLIENT_ID, MQTT_CLIENT_USER, MQTT_CLIENT_PASS, false)) {
      debugln("MQTT conectado.");
      debug_flush();
      mqtt.subscribe(MQTT_TOPIC_ROBOT_CMD);
      mqtt.loop();
      delay(10);
    }
  }
}

void messageReceived(String& topic, String& payload) {
  jsonError = deserializeJson(jsonDoc, payload);
  if (jsonError) return;

  debugln(payload);
  debug_flush();

  cmd = jsonDoc["cmd"];
}

void motor(int mleft, int mright) {
  int pwma = mleft >= 0 ? mleft : -mleft;
  int dira = mleft >= 0 ? HIGH : LOW;
  int pwmb = mright >= 0 ? mright : -mright;
  int dirb = mright >= 0 ? HIGH : LOW;

  digitalWrite(MOTOR_DIRA, dira);
  digitalWrite(MOTOR_DIRB, dirb);
  analogWrite(MOTOR_PWMA, pwma);
  analogWrite(MOTOR_PWMB, pwmb);
}
