# Testproject for a wide variety of microcontrollers

Support for ATTiny, Arduino Uno, Mega, and various ESP8266 and ESP32 boards with release and git-master SDKs

## Dependencies

[Platformio](https://platformio.org/), which installs: ArduinoJson, MQTT

## Build

```bash
pio init
pio run
pio run -t upload [-e <yourboard>]
```

## Customize network configuration

Copy `data/net.json.default` to `data/net.json` and add your settings.

```bash
pio run -t buildfs
pio run -t updatefs
```

## ESP32 notes

* In order to build MQTT for ESP32, this patch needs to be applied: https://github.com/knolleary/pubsubclient/pull/336
* SPIFFS filesystem: Use this [Arduino plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin) to upload the SPIFFS filesystem to ESP32.
