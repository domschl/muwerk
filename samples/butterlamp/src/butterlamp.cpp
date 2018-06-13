#if defined(__ESP__)

#define USE_SERIAL_DBG 1

#include <ArduinoJson.h>

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "ldr.h"

#include "neocandle.h"

#if defined(__ESP32__)
#define LED_BUILTIN (5)
#endif

ustd::Scheduler sched;
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
ustd::Ldr ldr("ldr", A0);
ustd::NeoCandle candles("candles", NEOCANDLE_PIN, NEOCANDLE_NUMPIXELS,
                        NEOCANDLEX_OPTIONS);

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

    sched.subscribe(SCHEDULER_MAIN, "butterlamp/#", subsMsg);
    sched.subscribe(SCHEDULER_MAIN, "+/luminosity", subsMsg);

#ifdef USE_SERIAL_DBG
    Serial.println("Starting net.begin");
#endif
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
    ldr.begin(&sched);
    candles.begin(&sched);

#ifdef USE_SERIAL_DBG
    Serial.println("Setup complete");
#endif
}

void loop() {
    sched.loop();
}

#endif  // __ESP__
