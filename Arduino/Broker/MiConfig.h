#ifndef _MI_CONFIG_H
#define _MI_CONFIG_H

#define NODO_ID "RETRO_SENSORS"

#define WIFI_SSID "ssid"
#define WIFI_PASS "pass"

#define MQTT_CLIENT_ID NODO_ID
#define MQTT_CLIENT_USER nullptr
#define MQTT_CLIENT_PASS nullptr

#define MQTT_SERVER "10.42.0.1"
#define MQTT_PORT 1883

#define MQTT_TOPIC_DHT22 "rcr/demos/dht22"
#define MQTT_TOPIC_MPL3115A2 "rcr/demos/mpl3115a2"
#define MQTT_TOPIC_NOVAPM "rcr/demos/novapm"
#define MQTT_TOPIC_ROBOT_CMD "rcr/demos/jacob/cmd"
#define MQTT_TOPIC_ROBOT_INFO "rcr/demos/jacob/info"

#endif  // _MI_CONFIG_H