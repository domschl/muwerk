// radio433cc.h
#pragma once

#include <Arduino.h>
#include <cc1101.h>
#include <ccpacket.h>

#include "scheduler.h"

// CC1101 433Mhz module

//  +-Vcc [ ][ ]Vcc -----------------+
//  |Mosi [ ][ ]Sck        TI-CC1101 |
//  |Miso [ ][ ]GD2    #             |##########
//  | Csn [ ][ ]GD0                  |
//  +-Gnd [ ][ ]Gnd -----------------+

// Arduino Uno:
// Attach CC1101 pins to their corresponding SPI pins
// Uno pins:
// CSN (SS) => 10
// MOSI => 11
// MISO => 12
// SCK => 13
// GD0 => A valid interrupt pin for your platform (defined below this)

// Connections between the CC1101 and the ESP8266 or Arduino:
// CC11xx pins    ESP pins Arduino pins  Description
// *  1 - VCC        VCC      VCC           3v3
// *  2 - GND        GND      GND           Ground
// *  3 - MOSI       13=D7    Pin 11        Data input to CC11xx
// *  4 - SCK        14=D5    Pin 13        Clock pin
// *  5 - MISO/GDO1  12=D6    Pin 12        Data output from CC11xx
//                                          serial clock from CC11xx
// *  6 - GDO2       04=D2    Pin  2        Programmable output
// *  7 - GDO0       ?        Pin  ?        Programmable output
// *  8 - CSN        15=D8    Pin 10        Chip select / (SPI_SS)

#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
#define CC1101Interrupt 4  // Pin 19
#define CC1101_GDO0 19
#elif defined(__MK64FX512__)
// Teensy 3.5
#define CC1101Interrupt 9  // Pin 9
#define CC1101_GDO0 9
#elif defined(ESP8266)
#define CC1101Interrupt D2  // Pin
#define CC1101_GDO0 D2
#else
#define CC1101Interrupt 0  // Pin 2
#define CC1101_GDO0 2
#endif

namespace ustd {

bool packetWaiting;
void messageReceived() {
    packetWaiting = true;
}

class Radio433CC {
  public:
    Scheduler *pSched;
    int tID;
    String name;
    bool state;

    // CC1101 stuff
    CC1101 *pRadio;
    byte syncWord[2] = {199, 10};
    unsigned long lastSend = 0;
    unsigned int sendDelay = 5000;
    // end CC1101

    Radio433CC(String name) : name(name) {
    }

    ~Radio433CC() {
    }

    // Get signal strength indicator in dBm.
    // See: http://www.ti.com/lit/an/swra114d/swra114d.pdf
    int rssi(char raw) {
        uint8_t rssi_dec;
        // TODO: This rssi_offset is dependent on baud and MHz; this is
        // for 38.4kbps and 433 MHz.
        uint8_t rssi_offset = 74;
        rssi_dec = (uint8_t)raw;
        if (rssi_dec >= 128)
            return ((int)(rssi_dec - 256) / 2) - rssi_offset;
        else
            return (rssi_dec / 2) - rssi_offset;
    }

    // Get link quality indicator.
    int lqi(char raw) {
        return 0x3F - raw;
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
        pSched->subscribe(tID, name + "/radio433cc/#", fnall);

        pRadio = new CC1101();
        pRadio->init();
        // pRadio->setSyncWord(syncWord);
        pRadio->setCarrierFreq(CFREQ_433);
        pRadio->disableAddressCheck();
        pRadio->setTxPowerAmp(PA_LongDistance);

        pRadio->setRxState();

        Serial.print(F("CC1101_PARTNUM "));
        Serial.println(pRadio->readReg(CC1101_PARTNUM, CC1101_STATUS_REGISTER));
        Serial.print(F("CC1101_VERSION "));
        Serial.println(pRadio->readReg(CC1101_VERSION, CC1101_STATUS_REGISTER));
        Serial.print(F("CC1101_MARCSTATE "));
        Serial.println(
            pRadio->readReg(CC1101_MARCSTATE, CC1101_STATUS_REGISTER) & 0x1f);

        Serial.println(F("CC1101 radio initialized."));

        attachInterrupt(CC1101Interrupt, messageReceived, FALLING);
    }

    int pkg = 0;

    void loop() {
        if (packetWaiting) {
            detachInterrupt(CC1101Interrupt);
            packetWaiting = false;
            ++pkg;
            char msg[256];
            sprintf(msg, "Pkg-nr: %d", pkg);
            Serial.println(msg);

            // pSched->publish(name + "/received", msg);
            CCPACKET packet;
            if (pRadio->receiveData(&packet) > 0) {
                // pSched->publish(name + "/received", "something");
                Serial.println(F("Received packet..."));
                if (!packet.crc_ok) {
                    Serial.println(F("crc not ok"));
                }
                Serial.print(F("lqi: "));
                Serial.println(lqi(packet.lqi));
                Serial.print(F("rssi: "));
                Serial.print(rssi(packet.rssi));
                Serial.println(F("dBm"));

                if (packet.crc_ok && packet.length > 0) {
                    Serial.print(F("packet: len "));
                    Serial.println(packet.length);
                    Serial.println(F("data: "));
                    Serial.println((const char *)packet.data);
                }
            }

            attachInterrupt(CC1101Interrupt, messageReceived, FALLING);
        }
        /*
        unsigned long now = millis();
        if (now > lastSend + sendDelay) {
            detachInterrupt(CC1101Interrupt);

            lastSend = now;
            const char *message = "hello world";
            CCPACKET packet;
            // We also need to include the 0 byte at the end of the string
            packet.length = strlen(message) + 1;
            strncpy((char *)packet.data, message, packet.length);

            pRadio->sendData(packet);
            Serial.println(F("Sent packet..."));

            attachInterrupt(CC1101Interrupt, messageReceived, FALLING);
        }
        */
    }

    void subsMsg(String topic, String msg, String originator) {
        if (topic == name + "/radio433cc/set") {
        }
    };
};  // Radio433CC

}  // namespace ustd
