#ifndef _MI_CONFIG_H
#define _MI_CONFIG_H

#define NODO_ID "Node_RCR_NOVAPM"
#define PIN_VOLTAJE A0
#define SSERIAL_RX 14  // D5
#define SSERIAL_TX 12  // D6
#define SONOFF 1       // apagar y encender el sds011 en cada ciclo
#define SWARMUP 30000  // ms de espera a que se estabilice el ventilador

#define DELAY_MS 10000

#define WIFI_SSID "ssid"
#define WIFI_PASS "pass"

#define MQTT_CLIENT_ID NODO_ID
#define MQTT_CLIENT_USER nullptr
#define MQTT_CLIENT_PASS nullptr

#define MQTT_SERVER "10.42.0.1"
#define MQTT_PORT 1883
#define MQTT_TOPIC "rcr/demos/novapm"

#endif  // _MI_CONFIG_H