#include "M5Stack.h"

#define USE_SERIAL_DBG 1

#include <time.h>  // time() ctime()

#include "platform.h"
#include "scheduler.h"
#include "net.h"
#include "mqtt.h"
#include "ota.h"

ustd::Scheduler sched;
ustd::Net net;
ustd::Mqtt mqtt;
ustd::Ota ota;

#if defined(__ESP32__)
#define LED_BUILTIN (5)
#endif

int led;
float nord_temp = -99.0;
time_t nord_temp_lastupd = 0;
float nord_hum = -99.0;
time_t nord_hum_lastupd = 0;
float balkon_temp = -99.0;
time_t balkon_temp_lastupd = 0;
float balkon_hum = -99.0;
time_t balkon_hum_lastupd = 0;

char baloon_msg[53];
time_t baloon_time = 0;
char static_msg[53];
time_t static_time = 0;

void netconnect(String topic, String msg, String originator) {
    digitalWrite(led, HIGH);  // Turn the LED on
}

void mqttMessages(String topic, String msg, String originator) {
#ifdef USE_SERIAL_DBG
    String me = topic + " " + msg + " " + originator;
    Serial.println(me.c_str());
#endif
    time_t now = time(nullptr);
    if (topic == "Nordseite/Temperature") {
        nord_temp = atof(msg.c_str());
        nord_temp_lastupd = now;
    }
    if (topic == "Nordseite/Humidity") {
        nord_hum = atof(msg.c_str());
        nord_hum_lastupd = now;
    }
    if (topic == "Balkon/Temperature") {
        balkon_temp = atof(msg.c_str());
        balkon_temp_lastupd = now;
    }
    if (topic == "Balkon/Humidity") {
        balkon_hum = atof(msg.c_str());
        balkon_hum_lastupd = now;
    }
    if (topic == "m5stack1/baloon") {
        strncpy(baloon_msg, msg.c_str(), 52);
        baloon_msg[52] = 0;
        baloon_time = now;
    }
    if (topic == "m5stack1/staticmsg") {
        strncpy(static_msg, msg.c_str(), 52);
        static_msg[52] = 0;
        static_time = now;
    }
}

void m5loop() {
    M5.update();
}

void buttonLoop() {
    if (M5.BtnA.wasPressed()) {
        M5.powerOFF();
    }
}

time_t oldnow = 0;
void displayTask() {
    char buf[256];
    time_t now = time(nullptr);
    if (now + static_time + baloon_time == oldnow)
        return;
    oldnow = now + static_time + baloon_time;
    M5.Lcd.setCursor(0, 32);
    M5.Lcd.setTextSize(2);
    if (now < 20)
        strcpy(buf, "Acquiring NTP time...");
    else if (now < 60)
        strcpy(buf, "Failed to acquire NTP time");
    else
        strncpy(buf, ctime(&now), 255);
    M5.Lcd.print(buf);
    if (nord_temp > -90.0) {
        M5.Lcd.setCursor(0, 64);
        M5.Lcd.setTextSize(2);
        sprintf(buf, "Nord: %4.1f C %3.0f%%\n", nord_temp, nord_hum);
        M5.Lcd.print(buf);
        M5.Lcd.setTextSize(1);
        if (nord_temp_lastupd < 60)
            sprintf(buf, "          [cached]\n");
        else
            sprintf(buf, "          %s\n", ctime(&nord_temp_lastupd));
        M5.Lcd.print(buf);
    }
    if (balkon_temp > -90.0) {
        M5.Lcd.setCursor(0, 96);
        M5.Lcd.setTextSize(2);
        sprintf(buf, "Sued: %4.1f C %3.0f%%\n", balkon_temp, balkon_hum);
        M5.Lcd.print(buf);
        M5.Lcd.setTextSize(1);
        if (balkon_temp_lastupd < 60)
            sprintf(buf, "          [cached]\n");
        else
            sprintf(buf, "          %s\n", ctime(&balkon_temp_lastupd));
        M5.Lcd.print(buf);
    }

    const char empty2lines[] =
        "                                                    ";
    if (now - baloon_time < 10) {
        strcpy(buf, empty2lines);
        strncpy(buf, baloon_msg, strlen(baloon_msg));
    } else
        strcpy(buf, empty2lines);
    M5.Lcd.setCursor(0, 140);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print(buf);
    if (static_time > 0) {
        strcpy(buf, empty2lines);
        strncpy(buf, static_msg, strlen(static_msg));
    } else
        strcpy(buf, empty2lines);
    M5.Lcd.setCursor(0, 172);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print(buf);

    M5.Lcd.setCursor(0, 220);
    M5.Lcd.setTextSize(2);
    sprintf(buf, " [On/Off] [Btn-B] [Btn-C]");
    M5.Lcd.print(buf);
    // sched.publish("ctime", ctime(&now));
}

void setup() {
#ifdef USE_SERIAL_DBG
    Serial.begin(115200);
    Serial.println();
    Serial.println("Startup");
#endif

    M5.begin();
    // M5.setWakeupButton(BUTTON_A_PIN);

    // LCD display
    M5.Lcd.println("Borg-kernel v2414.0.0.1");

    led = LED_BUILTIN;
    pinMode(led, OUTPUT);

    sched.subscribe(SCHEDULER_MAIN, "#", mqttMessages);
    sched.subscribe(SCHEDULER_MAIN, "net/rssi", netconnect);
    sched.add(displayTask, "disp", 50000L);  // 50ms
    sched.add(m5loop, "m5lp", 1000L);

    dacWrite(25, 0);  // prevent speaker-noise
    // sched.add(buttonLoop, "butn", 2000);

#ifdef USE_SERIAL_DBG
    Serial.println("Starting net.begin");
#endif
    net.begin(&sched);
    mqtt.begin(&sched);
    ota.begin(&sched);

#ifdef USE_SERIAL_DBG
    Serial.println("Setup complete");
#endif
}

void loop() {
    sched.loop();
}
