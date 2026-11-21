#ifndef _MI_CONFIG_H
#define _MI_CONFIG_H

#define NODO_ID "Node_RCR_DHT22"
#define PIN_VOLTAJE A0
#define PIN_DHT D5

#define DELAY_MS 10000

#define WIFI_SSID "ssid"
#define WIFI_PASS "pass"

#define MQTT_CLIENT_ID NODO_ID
#define MQTT_CLIENT_USER nullptr
#define MQTT_CLIENT_PASS nullptr

#define MQTT_SERVER "192.168.xxx.yyy"
#define MQTT_PORT 1883
#define MQTT_TOPIC "rcr/demos/dht22"

#endif  // _MI_CONFIG_H