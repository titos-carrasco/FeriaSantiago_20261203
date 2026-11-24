#ifndef _MI_CONFIG_H
#define _MI_CONFIG_H

#define NODO_ID "Node_RCR_MPL3115A2"
#define PIN_VOLTAJE A0

#define DELAY_MS 10000

#define WIFI_SSID "ssid"
#define WIFI_PASS "pass"

#define MQTT_CLIENT_ID NODO_ID
#define MQTT_CLIENT_USER nullptr
#define MQTT_CLIENT_PASS nullptr

#define MQTT_SERVER "10.42.0.1"
#define MQTT_PORT 1883
#define MQTT_TOPIC "rcr/demos/mpl3115a2"

#endif  // _MI_CONFIG_H