#include <ESP8266WiFi.h>
WiFiClient wifi;

#include <time.h>
const char* TZ_INFO = "CLT4CLST,M9.1.6/24,M4.1.6/24";

#include <ArduinoJson.h>
JsonDocument jsonDoc;
char buffer[512];

#include <MQTT.h>
MQTTClient mqtt;

#include "MiConfig.h"

#include <SoftwareSerial.h>
SoftwareSerial comm;

#include "SdsDustSensor.h"
SdsDustSensor sds(comm);

// inicializacion del programa
void setup() {
  // inicia monitor serial
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.flush();

  // inicializa la red
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // inicializa ntp
  configTime(TZ_INFO, "pool.ntp.org", "time.nist.gov");

  // inicializa mqtt
  mqtt.begin(MQTT_SERVER, MQTT_PORT, wifi);
  mqtt.loop();
  delay(10);

  // inicializa el SDS011
  comm.begin(9600, SWSERIAL_8N1, SSERIAL_RX, SSERIAL_TX, false);
  sds.begin();
  Serial.println(sds.queryFirmwareVersion().toString());   // prints firmware version
  Serial.println(sds.setQueryReportingMode().toString());  // ensures sensor is in 'query' reporting mode
  Serial.flush();

  // para leer el voltaje de la pila
  pinMode(PIN_VOLTAJE, INPUT);
}

// ciclo de ejecucion
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!mqtt.connected())
      mqtt.connect(MQTT_CLIENT_ID, MQTT_CLIENT_USER, MQTT_CLIENT_PASS, false);
    if (mqtt.connected()) {
      // limpiamos
      jsonDoc.clear();

      // id de este nodo
      jsonDoc["nodo_id"] = NODO_ID;

      // datos del sensor nova
      PmResult pm = sds.queryPm();
      float pm25 = pm.pm25;
      jsonDoc["pm25"] = pm25;
      float pm10 = pm.pm10;
      jsonDoc["pm10"] = pm10;

      // datos del voltaje
      jsonDoc["voltaje"] = round(analogRead(PIN_VOLTAJE) * (3.3 / 1023.0) * 100.0) / 100.0;

      // datos del reloj
      time_t now = time(nullptr);
      struct tm* timeinfo = localtime(&now);
      char timeBuffer[25];
      strftime(timeBuffer, sizeof(timeBuffer), "%d/%m/%Y %H:%M:%S", timeinfo);
      jsonDoc["datetime"] = timeBuffer;

      // para procesamiento interno
      yield();

      // formateamos y vemos la data a transmitir
      serializeJson(jsonDoc, buffer);
      Serial.print("Data: ");
      Serial.print(buffer);
      Serial.println();
      Serial.flush();

      // publicamos
      mqtt.publish(MQTT_TOPIC, buffer);
    } else {
      Serial.println("MQTT no conectado. Reintentando ...");
      Serial.flush();
    }
  } else {
    Serial.println("WiFi no conectada. Reintentando ...");
    Serial.flush();
  }

  mqtt.loop();
  delay(DELAY_MS);
}

void nova_onoff(bool on) {
  if (on) {
    Serial.print("Encendiendo y estabilizando el SDS011 ... ");
    Serial.flush();
    sds.wakeup();
    delay(SWARMUP);
    Serial.println("OK");
    Serial.flush();
  } else {
    Serial.print("Apagando el SDS011 ... ");
    Serial.flush();
    sds.sleep();
    delay(500);
    Serial.println("OK");
    Serial.flush();
  }
}
