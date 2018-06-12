# microWerk

microWerk is a test-version derived from [Meisterwerk](https://github.com/YeaSoft/MeisterWerk) by Leo Moll and Dominik Schösser.

## Current structure, which will change

* ustd (micro-stdlib), a minimal implementation of array, vector and map classes that work on all arduino platforms, from 8kb attiny up to Mac or Linux.
* muwerk (microWerk), a cooperative scheduler and an MQTT-like communication-queue for all arduino devices (attiny up to ESP)
* munet, modules for network connectivity for ESP8266 and ESP32 devices, implements Wireless connection to access point, NTP time protocol, OTA over-the-air udpate, MQTT-stack that connects to muwerk-queues.
* a number of implementations for sensors and io-devices
* samples

At some point in the future, what is now a single project will be split into independent subprojects: ustd and muwerk work with basically any micro-controller, where as munet and the samples are for ESP-chips.

This is unfinished work in progress...

## Licenses

This project uses the MIT license. Examples include external libraries. Each library is subject to their own license.
