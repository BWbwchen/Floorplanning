#include "loc.h"

#include <cmath>

#define F_THRESHOLD 0.0001

namespace std {

Loc Loc::operator+(const Loc &obj) {
    Loc c(x, y);
    c.x += obj.x;
    c.y += obj.y;
    c.z += obj.z;
    return c;
}
Loc Loc::operator-(const Loc &obj) {
    Loc c(x, y);
    c.x -= obj.x;
    c.y -= obj.y;
    c.z -= obj.z;
    return c;
}

Loc Loc::operator-(const intg &f) {
    Loc c(x, y);
    c.x -= f;
    c.y -= f;
    c.z -= f;
    return c;
}
Loc Loc::operator*(const Loc &obj) {
    Loc c(x, y);
    c.x *= obj.x;
    c.y *= obj.y;
    c.z *= obj.z;
    return c;
}
Loc Loc::operator*(const intg &f) {
    Loc c(x, y);
    c.x *= f;
    c.y *= f;
    c.z *= f;
    return c;
}

bool Loc::operator==(const Loc &obj) const {
    return obj.x == x && obj.y == y;
}

}  // namespace std
