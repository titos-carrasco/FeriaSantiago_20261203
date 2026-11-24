#ifndef _MI_CONFIG_H
#define _MI_CONFIG_H

#define NODO_ID "Node_RCR_JACOB"

#define MOTOR_PWMA D1
#define MOTOR_DIRA D3
#define MOTOR_PWMB D2
#define MOTOR_DIRB D4

#define CMD_STOP 'S'
#define CMD_FORWARD 'F'
#define CMD_BACKWARD 'B'
#define CMD_LEFT 'L'
#define CMD_RIGHT 'R'
#define CMD_INFO 'I'

#define DEBUG 0
#if DEBUG == 1
#define debug_begin(n) Serial.begin(n)
#define debug(n) Serial.print(n)
#define debugln(n) Serial.println(n)
#define debug_flush() Serial.flush();
#else
#define debug_begin(n)
#define debug(n)
#define debugln(n)
#define debug_flush()
#endif

#define WIFI_SSID "ssid"
#define WIFI_PASS "pass"

#define MQTT_CLIENT_ID NODO_ID
#define MQTT_CLIENT_USER nullptr
#define MQTT_CLIENT_PASS nullptr

#define MQTT_SERVER "10.42.0.1"
#define MQTT_PORT 1883
#define MQTT_TOPIC_ROBOT_CMD "rcr/demos/jacob/cmd"
#define MQTT_TOPIC_ROBOT_INFO "rcr/demos/jacob/info"

#endif  // _MI_CONFIG_H