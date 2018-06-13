# M5Stack tests

This test-code displays MQTT sensor values and messages and NTP-time on the M5Stack display.

## Dependencies

[Platformio](https://platformio.org/), which installs: ArduinoJson, MQTT (PubSubClient), M5Stack

## Build

(Currently, PubSubClient requires a patch as described below)

```bash
pio run
pio run -t upload
```

## Customize network configuration

Copy `data/net.json.default` to `data/net.json` and add your settings.

## ESP32 notes

* In order to build MQTT for ESP32, this patch needs to be applied: https://github.com/knolleary/pubsubclient/pull/336
* SPIFFS filesystem: Use this [Arduino plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin) to upload the SPIFFS filesystem to ESP32.
