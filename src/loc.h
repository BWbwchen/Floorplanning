#ifndef LOC_H
#define LOC_H

#include <cmath>
#include <ostream>
#include <sstream>

#include "type.h"

namespace std {

class Loc {
private:
    intg x;
    intg y;
    intg z;

public:
    Loc() : x(0), y(0), z(0){};
    Loc(intg _x, intg _y) : x(_x), y(_y), z(0){};
    intg get_x() { return x; }
    intg get_y() { return y; }
    void set_x(intg _x) { x = _x; }
    void set_y(intg _y) { y = _y; }
    string debug_dump() {
        stringstream ss;
        ss << "( " << x << ", " << y << " )" << endl;
        return ss.str();
    }

    Loc operator+(const Loc &);
    Loc operator-(const Loc &);
    Loc operator-(const intg &);
    Loc operator*(const Loc &);
    Loc operator*(const intg &);
    /* Loc operator/(const intg &); */
    bool operator==(const Loc &) const;
};

}  // namespace std
#endif /* LOC_H */
