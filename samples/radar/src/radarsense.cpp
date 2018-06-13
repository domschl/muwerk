#if defined(__ESP__)

#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "ldr.h"
#include "switch.h"

#if defined(__ESP32__)
#define LED_BUILTIN (5)
#endif

ustd::Scheduler sched;
ustd::Net net(LED_BUILTIN);  // During connection-attempts, onboard-led is on,
                             // after successful connection led is off (led is
                             // then used for radar-events).
ustd::Mqtt mqtt;
ustd::Ota ota;

// an (option) light-dependent-resistor (LDR) is connected to A0 using 10k
// pulldown.
ustd::Ldr ldr("ldr", A0);
// switch sends 'radar/event' messages on BOTH on/off state changes.
// radar device is connected to D3
ustd::Switch radar("radar", D3, 20, ustd::Switch::customtopic_t::BOTH,
                   "radar/event");

uint8_t radarLed;

void subsMsg(String topic, String msg, String originator) {
    if (topic == "radar/event") {
        if (msg == "off") {
            digitalWrite(radarLed, HIGH);
        }
        if (msg == "on") {
            digitalWrite(radarLed, LOW);
        }
    }
}

void setup() {
    radarLed = LED_BUILTIN;  // used for net connection-state (on until
                             // connected) *and* radar events

    sched.subscribe(SCHEDULER_MAIN, "radar/#", subsMsg);

    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);

    ldr.begin(&sched);
    radar.begin(&sched);
}

void loop() {
    sched.loop();
}

#endif  // __ESP__
