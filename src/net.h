#ifndef NET_H_
#define NET_H_

#include <memory>
#include <vector>

#include "pin.h"

namespace std {

class Net {
public:
    vector<Pin *> pin_array;

public:
    Net(vector<Pin *> v) : pin_array(move(v)) {}
    intg hpwl();
};

}  // namespace std


#endif  // NET_H_
