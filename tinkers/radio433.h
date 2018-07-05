// radio433.h
#pragma once

#include "scheduler.h"

namespace ustd {

class Radio433 {
  public:
    Scheduler *pSched;
    int tID;
    String name;
    bool state;
    uint8_t rcvPin;

    Radio433(String name, uint8_t rcvPin) : name(name), rcvPin(rcvPin) {
    }

    ~Radio433() {
    }

    void begin(Scheduler *_pSched) {
        pSched = _pSched;

        // give a c++11 lambda as callback scheduler task registration of
        // this.loop():
        std::function<void()> ft = [=]() { this->loop(); };
        tID = pSched->add(ft, name, 2000);

        std::function<void(String, String, String)> fnall =
            [=](String topic, String msg, String originator) {
                this->subsMsg(topic, msg, originator);
            };
        pSched->subscribe(tID, name + "/radio433/#", fnall);

        pinMode(rcvPin, INPUT_PULLUP);
    }

    int aborted = 0;
    int len = 0;
    int started = 0;
    void loop() {
        unsigned long pulse = pulseIn(rcvPin, LOW, 10000);
        if (pulse > 300) {
            if (pulse > 1500 &&
                pulse <
                    11000) {  // passendes Signal (zwischen 150ms und 11000ms) ?
                if (pulse < 2500 && started) {  // kleiner 250ms ? Dann LOW
                    Serial.print("0");
                    aborted = 0;
                    ++len;
                }
                if (pulse < 5000 && pulse > 3000 &&
                    started) {  // Zwischen 500ms und 1000ms dann HIGH
                    Serial.print("1");
                    aborted = 0;
                    ++len;
                }
                if (pulse > 8000) {  // Groesser 800ms dann Startsequenz !
                    if (len > 0) {
                        Serial.print("[");
                        Serial.print(len);
                        Serial.print("]");
                    }
                    started = 1;
                    Serial.println();
                    Serial.print("S");
                    aborted = 0;
                    len = 0;
                }
            } else {
                if (started) {
                    started = 0;
                    // Serial.print("X");
                    if (!aborted) {
                        aborted = 1;
                        if (len > 0) {
                            Serial.print("[");
                            Serial.print(len);
                            Serial.print("]");
                        }
                        Serial.println();
                        len = 0;
                    }
                }
            }
        }
    }

    void subsMsg(String topic, String msg, String originator) {
        if (topic == name + "/radio433/set") {
        }
    };
};  // Radio433

}  // namespace ustd
