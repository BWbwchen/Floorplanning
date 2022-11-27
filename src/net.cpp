#include "net.h"

#include <limits>

namespace std {


intg Net::hpwl() {
    intg x_min = numeric_limits<intg>::max();
    intg x_max = numeric_limits<intg>::min();
    intg y_min = numeric_limits<intg>::max();
    intg y_max = numeric_limits<intg>::min();
    for (auto &ptr : pin_array) {
        auto &l = ptr->loc;
        intg x = l.get_x();
        x_min = min(x_min, x);
        x_max = max(x_max, x);

        intg y = l.get_y();
        y_min = min(y_min, y);
        y_max = max(y_max, y);
    }

    return (x_max - x_min) + (y_max - y_min);
}


}  // namespace std
