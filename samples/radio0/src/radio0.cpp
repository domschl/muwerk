#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#if defined(__ESP32__)
#define LED_BUILTIN (5)
#endif

ustd::Scheduler sched;
ustd::Net net(LED_BUILTIN);  // During connection-attempts, onboard-led is on,
                             // after successful connection led is off (led is
                             // then used for radio-events).
ustd::Mqtt mqtt;
ustd::Ota ota;

uint8_t radioLed;

void subsMsg(String topic, String msg, String originator) {
    if (topic == "radio/event") {
    }
}

void setup() {
    radioLed = LED_BUILTIN;  // used for net connection-state (on until
                             // connected) *and* radio events

    sched.subscribe(SCHEDULER_MAIN, "radio/#", subsMsg);

    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
}

void loop() {
    sched.loop();
}
