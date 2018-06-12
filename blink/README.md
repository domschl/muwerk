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

Note for ESP32 users: Use this [Arduino plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin) to upload the SPIFFS filesystem to ESP32.
