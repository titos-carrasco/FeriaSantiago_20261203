# Monitor IoT para C64

* Basic funciona bien con 300bauds
* Para pruebas con el firmware AT configurar el ESP8266 con AT+UART_DEF=300,8,1,0,0
* Al usar el emulador Vice considerar:
  * Userport device: Userport RS232/Modem
  * RS232:
    * Enable userport RS232 emulation
    * RS-232 Interface type: RS-232, normal control line levels
    * Device: Serial1, Baud: 300
    * RS232 devices: Serial 1: /dev/ttyUSB0 o \\.\COM19, Baud 300

* En Windows:
  * Habilitar Zona con cobertura inalámbrica móvil
  * IP 192.168.137.*
  * Lanzar servicio mosquitto

* En Linux:
  * Usar router local
  * Lanzar servicio mosquitto

## Generación de imágenes HIR para el fondo

Las imágenes de fondo para el C64 se hiceron con GIMP:

* Resolución de 320x200
* Modo Indexado: Use black and white (1-bit) palete
* Fonts en [C64 True Type Fonts](https://style64.org/c64-truetype)
* Exportar a PNG
* En [png2c64](https://png2c64.app/):
  * Import el png
  * Modo Hi-Res (320x200)
  * Exportar a HIR
