# muWerk network libraries for ESP8266 and ESP32

```
+-------------+
|   munet     |  Access point client connection, NTP, OTA-update, MQTT (via PubSubClient)
+-------------+
|   muwerk    |  Cooperative scheduler and MQTT-like communication between scheduler tasks (pub/sub) 
+-------------+
|   ustd      |  Minimal implementations of Queue, Map (Dicts) and Arrays for ATtiny -> ESP32
+-------------+
| Arduino SDK |  Arduino SDK for ATtiny, Arduino Uno, Mega, ESP8266, ESP32 etc.
+-------------+
```

## Configuration
The network configuration is stored as a JSON file in the SPIFFS filesystems of ESP8266 and ESP32

`data/net.json`:

```json
{
"SSID":"my-network-SSID",
"password":"myS3cr3t",
"hostname":"myhost",
"services": [
    {"timeserver": "time.nist.gov"},
    {"dstrules": "CET-1CEST,M3.5.0,M10.5.0/3"},
    {"mqttserver": "my.mqtt.server"}
]
}
```

## Uploading of the configuration

### Update platformio and sdks

```bash
pio update
pio upgrade
```

### ESP8266 and ESP32

```bash
# create the spiffs filesystem
pio run -t buildfs
# upload to ESP8266 (and ESP32 with latest platformio version)
pio run -t uploadfs
```

### ESP32

* Support for SPIFFS `uploadfs` option of platformio with ESP32 is very recent (as of June 2018). Alternatively this [Arduino plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin) can be used to upload the SPIFFS filesystem to ESP32.

## Network protocols

* Automatic connection to an access point: munet uses the SSID and password to connect the ESP to the local wireless network. Automatic reconnection will be tried on network-failures. On repeated failures, the ESP chip will be rebootet.
* NTP: The library uses the new internal NTP time implementation. See test/netclock for samples on how to access local and UTC time. The Unix time rule defines the automatic DST rules and timezones.
* OTA: The munet library handles over-the-air software updates. Simply upload new images with `pio run -t upload --upload-port <hostname-of-esp>`. OTA-mode automatically switches the muwerk scheduler into single-task mode.
* MQTT: tbd.
