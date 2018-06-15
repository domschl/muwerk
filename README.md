# microWerk

microWerk is a test-version derived from [Meisterwerk](https://github.com/YeaSoft/MeisterWerk) by Leo Moll and Dominik Schösser. It changes:

* No inheritance: microWerk does not impose an object model to projects that use it. Coupling uses callbacks or C++11 functionals.
* Larger hardware scope: ustd and muwerk can be used with basically all Arduino boards.
* Lightweightness and layered functionality that allows independent usage.

## Current structure, which will change

* [ustd](ustd/) (micro-stdlib), a minimal implementation of array, vector and map classes that work on all arduino platforms, from 8kb attiny up to ESP32 and Unixoids Mac or Linux (for testing).
* [muwerk](muwerk/) (microWerk), a cooperative scheduler and an MQTT-like communication-queue for all arduino devices (attiny up to ESP32 [and Unixoids Mac or Linux for testing])
* [munet](munet/), modules for network connectivity for ESP8266 and ESP32 devices, implements Wireless connection to access point, NTP time protocol, OTA over-the-air udpate, MQTT-stack (using [PubSubClient](https://github.com/knolleary/pubsubclient)) that connects to muwerk-queues.
* [tinkers](tinkers/), a number of implementations for sensors and io-devices
* [test](test/), some basic tests for different hardware platforms and scheduler (AVR, ESP, Linux, macOS)
* [samples](samples/), a number of specific examples using scheduler and tinkers on ESP platforms

```
             +------------------------------+
             |           Apps               |  Samples
             +------------------------------+
             |         Tinkers              |  Sensors, IO-libs, reusable functional units
+------------+-------------+----------------+
|  Testcode  |             |  munet (ESPx)  |  Access point client connection, NTP, 
+--------------------------+----------------+    OTA-update, MQTT (via PubSubClient)
|            .   muwerk    .                |  Cooperative scheduler and  
+-------------------------------------------+    MQTT-like communication (pub/sub)
|            .    ustd     .                |  Minimal implementations of Queue, Map (Dicts),
+------------+-------------+----------------+    Arrays
| Mac, Linux | Arduino SDK | ESP8266/32 SDK |  OS and Arduino-Frameworks
+------------+-------------+----------------+
```

At some point in the future, what is now a single project will be split into independent subprojects: ustd and muwerk work with basically any micro-controller, where as munet and the samples are for ESP-chips.

This is unfinished work in progress...

## Required patches

### Issues and patches for ESP8266 and ESP32 boards

* munet uses the MQTT-stack [PubSubClient](https://github.com/knolleary/pubsubclient). The maximum default size of MQTT messages (default: 128bytes(!)) should be increased. In `PubSubClient.h` make sure that a sufficient block-size is defined, e.g. `#define MQTT_MAX_PACKET_SIZE 512`. ESP32 users additonally need to apply the patch below.

### Issues and patches for ESP32 based boards

* munet uses the MQTT-stack [PubSubClient](https://github.com/knolleary/pubsubclient). PubSubClient does not currently support ESP32, but support can be enabled with a simple patch: <https://github.com/knolleary/pubsubclient/pull/336>. 
* munet uses the SPIFFS filesystem of ESP8266 and ESP32 boards to store network configuration. Support for SPIFFS `uploadfs` option of platformio with ESP32 is very recent (as of June 2018). Alternatively this [Arduino plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin) can be used to upload the SPIFFS filesystem to ESP32.
* M5Stack buttons: In case of random button-pressed events, check: <https://github.com/m5stack/M5Stack/issues/52>

## Licenses

This project uses the MIT license. Examples include external libraries. Each library is subject to their own license.
