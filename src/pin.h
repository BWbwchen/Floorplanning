#ifndef PIN_H_
#define PIN_H_

#include <string>

#include "loc.h"

namespace std {

class Pin {
public:
    string name;
    Loc loc;

public:
    Pin() = default;
    Pin(string n, Loc l) {
        name = n;
        loc = l;
    }
};

}  // namespace std



#endif  // PIN_H_
