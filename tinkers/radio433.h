// radio433.h
#pragma once

#include <RFControl.h>

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
        tID = pSched->add(ft, name, 40000);

        std::function<void(String, String, String)> fnall =
            [=](String topic, String msg, String originator) {
                this->subsMsg(topic, msg, originator);
            };
        pSched->subscribe(tID, name + "/radio433/#", fnall);

        RFControl::startReceiving(rcvPin);
    }

    void loop() {
        if (RFControl::hasData()) {
            unsigned int *timings;
            unsigned int timings_size;
            unsigned int pulse_length_divider =
                RFControl::getPulseLengthDivider();
            RFControl::getRaw(&timings, &timings_size);
            for (int i = 0; i < timings_size; i++) {
                unsigned long timing = timings[i] * pulse_length_divider;
#ifdef USE_SERIAL_DBG
                Serial.print(timing);
                Serial.write('\t');
                if ((i + 1) % 16 == 0) {
                    Serial.write('\n');
                }
#endif
            }
#ifdef USE_SERIAL_DBG
            Serial.write('\n');
            Serial.write('\n');
#endif
            RFControl::continueReceiving();
        }
    }

    void subsMsg(String topic, String msg, String originator) {
        if (topic == name + "/radio433/set") {
        }
    };
};  // Radio433

}  // namespace ustd
