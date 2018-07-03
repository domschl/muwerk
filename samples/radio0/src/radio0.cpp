#define USE_SERIAL_DBG 1

#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "radio433.h"

#if defined(__ESP32__)
#define LED_BUILTIN (5)
#endif

ustd::Scheduler sched;
ustd::Net net(LED_BUILTIN);  // During connection-attempts, onboard-led is on,
                             // after successful connection led is off (led is
                             // then used for radio-events).
ustd::Mqtt mqtt;
ustd::Ota ota;

ustd::Radio433 r433("Radio", 0);

void subsMsg(String topic, String msg, String originator) {
    if (topic == "radio/event") {
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("Hello, world!");

    sched.subscribe(SCHEDULER_MAIN, "radio/#", subsMsg);

    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);

    r433.begin(&sched);
}

void loop() {
    sched.loop();
}
