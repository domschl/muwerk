#define USE_SERIAL_DBG 1

#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

#include "oled.h"

#include <Wire.h>

#if defined(__ESP32__)
#define LED_BUILTIN (5)
#endif

ustd::Scheduler sched;
ustd::Net net(LED_BUILTIN);  // During connection-attempts, onboard-led is on,
                             // after successful connection led is off (led is
                             // then used for radar-events).
ustd::Mqtt mqtt;
ustd::Ota ota;

ustd::Oled oled("ssd1306", 0x3c, true, "tsl2561/unitluminosity");

void subsMsg(String topic, String msg, String originator) {
}

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
#endif
    pinMode(16, OUTPUT);
    digitalWrite(16, LOW);  // set GPIO16 low to reset OLED
    delay(50);
    digitalWrite(16, HIGH);  // while OLED is running, must set GPIO16 in high

    Wire.begin(4, 15);  // SDA, SCL; Non-standard
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);

    oled.begin(&sched);
}

void loop() {
    sched.loop();
}
