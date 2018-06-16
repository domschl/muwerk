#include "scheduler.h"
#include "array.h"

namespace ustd {
// Form: This is a sample Form\nCpu: %5s Temp: %5s\n
// Vars: ["val/cpu", "bla/temperature"];
// [{0,0,1,"@time"},{1,0,1,"Cpu:"}]
// "Cpu {topic/cpu} {@time}"
// "T1 {topic/temperature} T2 {topic2/temperature}"
class TextTrans {
  public:
    // ustd::map<String, String> topic2value;
    ustd::array<String> topics;

    TextTrans() {
    }
    ~TextTrans() {
    }

    // get all substrings delimited by c1,c2 from source and put them
    // into array pDest. Return number of tokens found.
    // e.g. source="asd {a} {abc} {def} sdf" with c1,c2 = '{', '}' should
    // give array ["a","abc","def"]
    int getDelimitedTokens(String source, char c1, char c2,
                           ustd::array<String> *pDest) {
        bool finished = false;
        int i1, i2;
        String src = source;
        int nr = 0;
        while (!finished) {
            i1 = src.indexOf(c1);
            i2 = src.substring(i1 + 1).indexOf(c2);
            if ((i1 == -1) || (i2 == -1)) {
                finished = true;
            } else {
                String sub = src.substring(i1 + 1, i2 + i1 + 1);
                src = src.substring(i1 + 1 + i2 + 1);
                pDest->add(sub + "|" + src);
                ++nr;
            }
        }
        return nr;
    }

    int registerForm(String form) {
        while (!topics.isEmpty()) {
            topics.erase(0);
        }
        return getDelimitedTokens(form, '{', '}', &topics);
    }
};
}  // namespace ustd