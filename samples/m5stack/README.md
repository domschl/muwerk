# M5Stack tests

This test-code displays MQTT sensor values and messages and NTP-time on the M5Stack display.

## Dependencies

[Platformio](https://platformio.org/), which installs: ArduinoJson, MQTT (PubSubClient), M5Stack

## Build

(Currently, PubSubClient requires a patch as described below)

```bash
# First time configure network (after copying and configuring `data/net.json.default` to `data/net.json`)
pio run -t buildfs
pio run -t uploadfs
# Update software:
pio run -t upload
# As soon as the first image is installed, OTA programming can be used:
pio run -t upload --upload-port <mym5stack.home.net>
```

## Customize network configuration

Copy `data/net.json.default` to `data/net.json` and add your settings.

## ESP32 notes

* In order to build MQTT for ESP32, this patch needs to be applied: https://github.com/knolleary/pubsubclient/pull/336
* Support for SPIFFS `uploadfs` option of platformio with ESP32 is very recent (as of June 2018). Alternatively this [Arduino plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin) can be used to upload the SPIFFS filesystem to ESP32.
* M5Stack buttons: In case of random button-pressed events, check: <https://github.com/m5stack/M5Stack/issues/52>

