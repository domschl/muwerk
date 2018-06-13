# microWerk

microWerk is a test-version derived from [Meisterwerk](https://github.com/YeaSoft/MeisterWerk) by Leo Moll and Dominik Schösser. It changes:

* No inheritance: microWerk does not impose an object model to projects that use it. Coupling uses callbacks or C++11 functionals.
* Larger hardware scope: ustd and muwerk can be used with basically all Arduino boards.
* Lightweightness and layered functionality that allows independent usage.

## Current structure, which will change

* ustd (micro-stdlib), a minimal implementation of array, vector and map classes that work on all arduino platforms, from 8kb attiny up to Mac or Linux.
* muwerk (microWerk), a cooperative scheduler and an MQTT-like communication-queue for all arduino devices (attiny up to ESP)
* munet, modules for network connectivity for ESP8266 and ESP32 devices, implements Wireless connection to access point, NTP time protocol, OTA over-the-air udpate, MQTT-stack (using [PubSubClient](https://github.com/knolleary/pubsubclient)) that connects to muwerk-queues.
* tinkers, a number of implementations for sensors and io-devices
* test, some basic tests for different hardware platforms (AVR and ESP) and scheduler
* samples, a number of specific examples using scheduler and tinkers on ESP platforms

At some point in the future, what is now a single project will be split into independent subprojects: ustd and muwerk work with basically any micro-controller, where as munet and the samples are for ESP-chips.

This is unfinished work in progress...

## Required patches

### Issues and patches for ESP32 based boards

* munet uses the MQTT-stack [PubSubClient](https://github.com/knolleary/pubsubclient). PubSubClient does not currently support ESP32, but support can be enabled with a simple patch: <https://github.com/knolleary/pubsubclient/pull/336>
* munet uses the SPIFFS filesystem of ESP8266 and ESP32 boards to store network configuration. Platformio doesn't (currently, to my knowledge) support upload of the SPIFFS filesystem to ESP32 boards. Use this [Arduino plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin) to upload the SPIFFS filesystem to ESP32.
* M5Stack buttons: In case of random button-pressed events, check: <https://github.com/m5stack/M5Stack/issues/52>

## Licenses

This project uses the MIT license. Examples include external libraries. Each library is subject to their own license.
