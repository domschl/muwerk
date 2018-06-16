#define USE_SERIAL_DBG 1

#include "platform.h"

#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#if defined(__ESP32__)
#define LED_BUILTIN (5)
#endif

// add missing definitions for Adafruit Huzzah chip
#if defined(HUZZAH)
static const uint8_t D0 = 16;
static const uint8_t D1 = 5;
static const uint8_t D2 = 4;
static const uint8_t D3 = 0;
static const uint8_t D4 = 2;
static const uint8_t D5 = 14;
static const uint8_t D6 = 12;
static const uint8_t D7 = 13;
static const uint8_t D8 = 15;
static const uint8_t RX = 3;
static const uint8_t TX = 1;
#endif

#include "clock7seg.h"
#include "lumin.h"

#include <ESP8266WiFi.h>
#include "dhtxx.h"
#include "pressure.h"
#include "switch.h"
#include "airqual.h"
#include "oled.h"

ustd::Scheduler sched;
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
// autbrightness=true, look for tsl2561/unitluminosity messages to adapt
// brightness.
ustd::Clock7Seg clock7("clock", 0x70, D5, true, "tsl2561/unitluminosity");
ustd::Lumin lumin("tsl2561", 0x39, "1x", "fast", 28.0);
ustd::Dht dht("dht22", 0, DHT22);  // port 0 == D3
ustd::Pressure pressure("bmp085");
ustd::Switch sw1("sw1", D7);
ustd::Switch sw2("sw2", D6, 20, ustd::Switch::customtopic_t::ON,
                 "clock/alarm/off");
ustd::AirQuality airq("ccs811");
ustd::Oled oled("ssd1306", 0x3c, true, "tsl2561/unitluminosity");

void subsMsg(String topic, String msg, String originator) {
#ifdef USE_SERIAL_DBG
    String me = topic + " " + msg + " " + originator;
    Serial.println(me.c_str());
#endif
}

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println();
    Serial.println("Startup");
#endif

#if defined(I2C_D1_D2)
#ifdef USE_SERIAL_DBG
    Serial.println("Using slightly non-standard I2C port D1, D2");
#endif
    Wire.begin(D1, D2);  // SDA, SCL; Non-standard, from the old days...
#endif
    Wire.setClock(400000L);

    // CCS811 has the tendency to lock up the I2C bus on ESP platforms
    // Workaround: relax clock stretch limit from 230 (default) to 460.
    // See: https://forums.adafruit.com/viewtopic.php?f=19&t=121816&start=15
    // twi_setClockStretchLimit(460);
    Wire.setClockStretchLimit(460);
    sched.subscribe(SCHEDULER_MAIN, "borgclock/#", subsMsg);

    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    clock7.begin(&sched);
    clock7.maxAlarmDuration = 10;
    lumin.begin(&sched);
    dht.begin(&sched);
    pressure.begin(&sched);
    sw1.begin(&sched);
    sw2.begin(&sched);
    airq.begin(&sched);
    oled.begin(&sched);
}

void loop() {
    sched.loop();
}
