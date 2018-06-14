#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "scheduler.h"
#include "texttrans.h"

// Oled display default i2c address:
#define OLED_DISPLAY_ADDRESS 0x3C

namespace ustd {
class Oled {
  public:
    Scheduler *pSched;
    int tID;
    String name;
    uint8_t i2cAddress;
    bool bStarted = false;
    Adafruit_SSD1306 *pOled;
    bool bAutobrightness = true;
    String brightnessTopic = "";

    Oled(String name, uint8_t i2cAddress = OLED_DISPLAY_ADDRESS,
         bool bAutobrightness = true, String brightnessTopic = "")
        : name(name), i2cAddress(i2cAddress), bAutobrightness(bAutobrightness),
          brightnessTopic(brightnessTopic) {
    }

    ~Oled() {
    }

    void setBrightness(double fLevel) {  // 0.0 - 1.0
        double brL = fLevel;
        if (brL < 0.0)
            brL = 0.0;
        if (brL > 1.0)
            brL = 1.0;
        if (brL < 0.3)
            pOled->dim(true);
        else
            pOled->dim(false);
    }

    void begin(Scheduler *_pSched) {
        pSched = _pSched;

        pOled = new Adafruit_SSD1306();
        pOled->begin(SSD1306_SWITCHCAPVCC,
                     i2cAddress);  // initialize with the I2C addr 0x3D (for the
                                   // 128x64) pOled->clear();
        pOled->clearDisplay();
        pOled->setTextSize(1);
        pOled->setTextColor(WHITE);
        pOled->setCursor(0, 0);
        pOled->println("123456789012345678901234567890");
        pOled->println("123456789012345678901234567890");
        pOled->println("123456789012345678901234567890");
        pOled->println("123456789012345678901234567890");
        pOled->println("123456789012345678901234567890");
        pOled->println("123456789012345678901234567890");
        pOled->println("123456789012345678901234567890");
        pOled->println("123456789012345678901234567890");
        pOled->println("123456789012345678901234567890");
        pOled->println("123456789012345678901234567890");

        pOled->display();

        std::function<void()> ft = [=]() { this->loop(); };
        tID = pSched->add(ft, name, 50000);

        std::function<void(String, String, String)> fnall =
            [=](String topic, String msg, String originator) {
                this->subsMsg(topic, msg, originator);
            };

        pSched->subscribe(tID, name + "/display/set", fnall);
        pSched->subscribe(tID, name + "/#", fnall);
        if (bAutobrightness) {
            if (brightnessTopic != "")
                pSched->subscribe(tID, brightnessTopic, fnall);
        }
        bStarted = true;
    }

    void loop() {
        if (bStarted) {
        }
    }

    void subsMsg(String topic, String msg, String originator) {
        if (msg == "dummyOn") {
            return;  // Ignore, homebridge hack
        }
        if (topic == name + "/display/set") {
#ifdef USE_SERIAL_DBG
            Serial.print("Message arrived [");
            Serial.print(topic.c_str());
            Serial.println("] ");
#endif
        }
        if (bAutobrightness) {
            if (pSched->mqttmatch(topic, brightnessTopic)) {
                double unitBrightness = atof(msg.c_str());
                setBrightness(unitBrightness);
            }
        }
    }
};
};  // namespace ustd
