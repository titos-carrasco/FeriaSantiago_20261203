# Monitor IoT para Atari 800XL

Basic funciona bien con 300bauds:

* Para pruebas con el firmware AT configurar el ESP8266 con AT+UART_DEF=300,8,1,0,0

Al usar e emulador Altirra considerar:

* Crear los siguientes dispositivos:
  * SIO Serial adapter -> Networked serial on port 9000/tcp at 300 baud
  * Host device (H:) apuntando al directorio Atari800XL (para usar ENTER H6:SENSORS.LST")
  * En MSYS2/Linux ejecutar: socat -v FILE:/dev/ttyS18,b300,raw,echo=0,crtscts=0 TCP:127.0.0.1:9000,retry=10,interval=1

Algunos links:

* https://github.com/dmlloyd/atari800/blob/master/DOC/rdevice_faq.txt
* https://www.atari.org.pl/charsetter/
