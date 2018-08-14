# Borgclock looks like an old-style 7-segment clock

...but via MQTT the following sensors are exported

* DHT22, temperature, humidity
* BMP085, air pressure, temperature
* CCS811, airquality: co2 and voc contamination
* TSL1562, luminosity

additionally, it has a small oled display for additional information and two buttons.

## Required patches

* https://github.com/sparkfun/SparkFun_CCS811_Arduino_Library/issues/6
* Adafruit SDD1306: adapt display size, `#define SSD1306_128_64`
* CCS811 together with ESP chips tend to lock up the I2C bus after some time.  in `.platformio/packages/framework-arduinoespressif8266/cores/esp8266` edit `core_esp8266_si2c.c` and change `twi_setClockStretchLimit(230);` from `230` to `460`. From discussion at <https://forums.adafruit.com/viewtopic.php?f=19&t=121816&start=15>.
