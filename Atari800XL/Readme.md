# Monitor IoT para Atari 800XL

* Basic funciona bien con 300bauds
* Para pruebas con el firmware AT configurar el ESP8266 con AT+UART_DEF=300,8,1,0,0
* Al usar e emulador Altirra considerar:
  * Crear los siguientes dispositivos:
    * SIO Serial adapter -> Networked serial on port 9000/tcp at 300 baud
    * Host device (H:) apuntando al directorio Atari800XL/basic
    * En MSYS2/Linux ejecutar:
      * ```socat -v FILE:/dev/ttySXX,b300,raw,echo=0,crtscts=0 TCP:127.0.0.1:9000,retry=10,interval=1```
      * En MSYS2 ttyS18 es COM19
  * Mantener montada la imagen disk/RCR.atr en D1:
  * Cargar el programa con
    * ```ENTER "H6:SENSORS.LST"```
    * Grabar en el ATR con ```goto 9999```
* En Windows:
  * Habilitar Zona con cobertura inalámbrica móvil
  * IP 192.168.137.*
  * Lanzar servicio mosquitto

* En Linux:
  * Usar router local
  * Lanzar servicio mosquitto

* Algunos links:
  * https://github.com/dmlloyd/atari800/blob/master/DOC/rdevice_faq.txt
  * https://www.atari.org.pl/charsetter/
