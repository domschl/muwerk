#if defined(__ESP__)

#define USE_SERIAL_DBG 1

#include <time.h>  // time() ctime()

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"
#include "web.h"

#if defined(__ESP32__)
#define LED_BUILTIN (5)
#endif

ustd::Scheduler sched;
ustd::Net net(LED_BUILTIN);
ustd::Mqtt mqtt;
ustd::Ota ota;
#if !defined(__ESP32__)
ustd::Web web;
#endif

void netconnect(String topic, String msg, String originator) {
    // digitalWrite(led, HIGH);  // Turn the LED off
}

void task0(String topic, String msg, String originator) {
#ifdef USE_SERIAL_DBG
    String me = topic + " " + msg + " " + originator;
    Serial.println(me.c_str());
#endif
}

#ifdef USE_SERIAL_DBG
#define PTM(w)                                                                 \
    Serial.print(":" #w "=");                                                  \
    Serial.print(tm->tm_##w);

void printTm(const char *what, const tm *tm) {
    Serial.print(what);
    PTM(isdst);
    PTM(yday);
    PTM(wday);
    PTM(year);
    PTM(mon);
    PTM(mday);
    PTM(hour);
    PTM(min);
    PTM(sec);
}
#endif

void task1() {
    time_t now = time(nullptr);
    sched.publish("ctime", ctime(&now));
#ifdef USE_SERIAL_DBG
    printTm("localtime", localtime(&now));
    Serial.println();
    printTm("gmtime   ", gmtime(&now));
    Serial.println();
#endif
}

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println();
    Serial.println("Startup");
#endif

    sched.subscribe(SCHEDULER_MAIN, "#", task0);
    sched.subscribe(SCHEDULER_MAIN, "net/rssi", netconnect);
    sched.add(task1, "task1", 10000000L);  // 10s

#ifdef USE_SERIAL_DBG
    Serial.println("Starting net.begin");
#endif
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);
#if !defined(__ESP32__)
    web.begin(&sched);
#endif

#ifdef USE_SERIAL_DBG
    Serial.println("Setup complete");
#endif
}

void loop() {
    sched.loop();
}

#endif  // __ESP__
