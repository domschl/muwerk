#include "scheduler.h"

// Form: This is a sample Form\nCpu: %5s Temp: %5s\n
// Vars: ["val/cpu", "bla/temperature"];
// [{0,0,1,"@time"},{1,0,1,"Cpu:"}]
// "Cpu {topic/cpu} {@time}"
// "T1 {topic/temperature} T2 {topic2/temperature}"
class TextTrans {
  public:
    ustd::map<String, String> topic2value;

    TextTrans() {
    }
    ~TextTrans() {
    }

    void registerForm(String form) {
    }
};