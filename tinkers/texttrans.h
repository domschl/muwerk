#include "scheduler.h"
#include "array.h"

namespace ustd {
// Form: This is a sample Form\nCpu: %5s Temp: %5s\n
// Vars: ["val/cpu", "bla/temperature"];
// [{0,0,1,"@time"},{1,0,1,"Cpu:"}]
// "Cpu {topic/cpu} {@time}"
// "@1,1,1:T1 {topic/temperature} T2 {topic2/temperature}"
class TextTrans {
  public:
    ustd::map<String, String> topic2value;
    ustd::array<String> topics;
    String form;

    TextTrans() {
    }
    ~TextTrans() {
    }

    String replace(String source, String token, String target) {
        String dst = source;
        bool finished = false;
        while (!finished) {
            finished = true;
            for (unsigned int i = 0; i < source.length() - token.length();
                 i++) {
                if (dst.substring(i, i + token.length()) == token) {
                    dst = dst.substring(0, i) + target +
                          dst.substring(i + token.length());
                    finished = false;
                    break;
                }
            }
        }
        return dst;
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
                pDest->add(sub);
                ++nr;
            }
        }
        return nr;
    }

    int registerForm(String newform) {
        while (!topics.isEmpty()) {
            topics.erase(0);
        }
        form = newform;
        return getDelimitedTokens(newform, '{', '}', &topics);
    }

    void registerValue(String topic, String value) {
        for (unsigned int i = 0; i < topics.length(); i++) {
            if (topics[i] == topic) {
                topic2value[topic] = value;
            }
        }
    }

    String fillForm() {
        String fform = form;
        String key, value;
        for (unsigned int i = 0; i < topics.length(); i++) {
            key = '{' + topics[i] + '}';
            if (topic2value.find(topics[i]) != -1) {
                value = topic2value[topics[i]];
            } else {
                value = "[?]";
            }
            fform = replace(fform, key, value);
        }
        return fform;
    }

    void screenSplitter(String info, ustd::array<String> *lines) {
        }
};
}  // namespace ustd