#define USE_SERIAL_DBG 1

#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

// #define R433_RAW

#ifdef R433_RAW
#include "radio433.h"
#else
#include "radio433rcs.h"
#endif

#if defined(__ESP32__)
#define LED_BUILTIN (5)
#endif

ustd::Scheduler sched;
ustd::Net net(LED_BUILTIN);  // During connection-attempts, onboard-led is on,
                             // after successful connection led is off (led is
                             // then used for radio-events).
ustd::Mqtt mqtt;
ustd::Ota ota;

#ifdef R433_RAW
ustd::Radio433 r433("Radio", D3);
#else
// Do NOT use D3: a hardware pull-up prevents RXB6 from working:
ustd::Radio433rcs r433("rxb6", D2);
#endif

void subsMsg(String topic, String msg, String originator) {
    if (topic == "radio/event") {
    }
}

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println("Hello, world!");
#endif
    sched.subscribe(SCHEDULER_MAIN, "radio/#", subsMsg);

    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);

    r433.begin(&sched);
}

void loop() {
    sched.loop();
}
