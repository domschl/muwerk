// radio433rcs.h
#pragma once
// Hacked from http://code.google.com/p/rc-switch/

#include <RCSwitch.h>

#include "scheduler.h"

namespace ustd {

class Radio433rcs {
  public:
    Scheduler *pSched;
    int tID;
    String name;
    bool state;
    uint8_t rcvPin;
    RCSwitch *pRadio;
    unsigned long count = 0;

    Radio433rcs(String name, uint8_t rcvPin) : name(name), rcvPin(rcvPin) {
    }

    ~Radio433rcs() {
    }

    void begin(Scheduler *_pSched) {
        pSched = _pSched;

        // give a c++11 lambda as callback scheduler task registration of
        // this.loop():
        std::function<void()> ft = [=]() { this->loop(); };
        tID = pSched->add(ft, name, 50);

        std::function<void(String, String, String)> fnall =
            [=](String topic, String msg, String originator) {
                this->subsMsg(topic, msg, originator);
            };
        pSched->subscribe(tID, name + "/radio433rcs/#", fnall);

        pinMode(rcvPin, INPUT);
        pRadio = new RCSwitch();
        pRadio->enableReceive(rcvPin);  // Receiver on interrupt rcvPin

        Serial.println("rf_sniffer started");
    }

    void loop() {
        if (pRadio->available()) {

            int value = pRadio->getReceivedValue();

            if (value == 0) {
                Serial.print("#### RECEIVE 433: Unknown encoding");
            } else {
                Serial.print("#### RECEIVE 433:  ");
                Serial.print(pRadio->getReceivedValue());
                Serial.print(" / ");
                Serial.print(pRadio->getReceivedBitlength());
                Serial.print("bit ");
                Serial.print("Protocol: ");
                Serial.println(pRadio->getReceivedProtocol());
            }
            pRadio->resetAvailable();
            count = 0;
        } else {
            if (++count % 30000 == 0)
                Serial.println("433: no activity");
        }
    }

    void subsMsg(String topic, String msg, String originator) {
        if (topic == name + "/radio433rcs/set") {
        }
    };
};  // Radio433rcs

}  // namespace ustd
