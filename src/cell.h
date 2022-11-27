#ifndef CELL_H_
#define CELL_H_

#include <memory>
#include <string>
#include <vector>

#include "loc.h"
#include "pin.h"

namespace std {

enum class SpecialCell {
    kHorizontal = -2,
    kVertical = -1,
    kCell = 0,
};


class Cell;

bool is_special_node(intg e);
void stockmeyer_vertical(Cell *c);
void stockmeyer_horizontal(Cell *c);

class CellRecord {
public:
    intg width;
    intg height;

    intg left_version;
    intg right_version;

public:
    CellRecord(intg w, intg h, intg lv, intg rv)
        : width(w), height(h), left_version(lv), right_version(rv){};
};

class Cell {
public:
    string name;
    Loc loc;
    intg width;
    intg height;
    bool rotate;

    Pin *p;

    // For Horizontal, Vertical node
    SpecialCell type;
    Cell *right;
    Cell *left;

    vector<CellRecord> record;

    friend void stockmeyer_vertical(Cell *c);
    friend void stockmeyer_horizontal(Cell *c);

public:
    Cell() = default;
    Cell(string n, intg w, intg h, Pin *pp) {
        name = n;
        loc = Loc(0, 0);
        width = w;
        height = h;
        rotate = false;
        type = SpecialCell::kCell;
        p = pp;

        right = nullptr;
        left = nullptr;

        record.emplace_back(CellRecord(width, height, 0, 0));
        record.emplace_back(CellRecord(height, width, 1, 1));
    }
    Cell(SpecialCell t, Cell *r, Cell *l) : type(t), right(r), left(l) {}
    intg get_area() const { return width * height; }
    void dp(Cell *r, Cell *l);
    void update_cell(intg version, Loc l);
};



}  // namespace std


#endif  // CELL_H_
