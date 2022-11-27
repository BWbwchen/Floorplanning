#include "cell.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>


namespace std {

bool is_special_node(intg e) {
    return e == -2 || e == -1;
}

bool sort_vertical(const CellRecord &lhs, const CellRecord &rhs) {
    return lhs.height > rhs.height;
}
bool sort_horizontal(const CellRecord &lhs, const CellRecord &rhs) {
    return lhs.width > rhs.width;
}

void stockmeyer_vertical(Cell *c) {
    sort(c->left->record.begin(), c->left->record.end(), sort_vertical);
    sort(c->right->record.begin(), c->right->record.end(), sort_vertical);

    intg l = 0;
    intg r = 0;
    intg l_limit = c->left->record.size();
    intg r_limit = c->right->record.size();
    const auto &l_record = c->left->record;
    const auto &r_record = c->right->record;

    c->record.clear();

    while (l < l_limit && r < r_limit) {
        c->record.emplace_back(
            CellRecord(l_record[l].width + r_record[r].width,
                       max(l_record[l].height, r_record[r].height), l, r));

        if (l_record[l].height < r_record[r].height) {
            r++;
        } else if (l_record[l].height > r_record[r].height) {
            l++;
        } else {
            r++;
            l++;
        }
    }
}
void stockmeyer_horizontal(Cell *c) {
    sort(c->left->record.begin(), c->left->record.end(), sort_horizontal);
    sort(c->right->record.begin(), c->right->record.end(), sort_horizontal);

    intg l = 0;
    intg r = 0;
    intg l_limit = c->left->record.size();
    intg r_limit = c->right->record.size();
    const auto &l_record = c->left->record;
    const auto &r_record = c->right->record;

    c->record.clear();

    while (l < l_limit && r < r_limit) {
        c->record.emplace_back(
            CellRecord(max(l_record[l].width, r_record[r].width),
                       l_record[l].height + r_record[r].height, l, r));

        if (l_record[l].width < r_record[r].width) {
            r++;
        } else if (l_record[l].width > r_record[r].width) {
            l++;
        } else {
            r++;
            l++;
        }
    }
}

void Cell::dp(Cell *r, Cell *l) {
    if (type == SpecialCell::kCell) {
        throw std::logic_error("Type cell cannot use dp function");
        return;
    }

    right = r;
    left = l;
    record.clear();
    if (type == SpecialCell::kVertical) {
        stockmeyer_vertical(this);
    } else if (type == SpecialCell::kHorizontal) {
        stockmeyer_horizontal(this);
    } else {
        right = nullptr;
        left = nullptr;
        throw std::logic_error("Unknown cell type in dp");
        return;
    }
}


void Cell::update_cell(intg version, Loc l) {
    if (type == SpecialCell::kCell) {
        this->loc = l;
        this->rotate = !(record[version].width == width &&
                         record[version].height == height);
        if (this->rotate) {
            p->loc = l + Loc(height / 2, width / 2);
        } else {
            p->loc = l + Loc(width / 2, height / 2);
        }
    } else {
        intg left_child_version = record[version].left_version;
        intg right_child_version = record[version].right_version;

        left->update_cell(left_child_version, l);

        Loc offset = Loc(0, 0);
        if (type == SpecialCell::kVertical) {
            offset.set_x(left->record[left_child_version].width);
        } else if (type == SpecialCell::kHorizontal) {
            offset.set_y(left->record[left_child_version].height);
        } else {
            throw std::logic_error("Unknown cell type in update_cell");
            return;
        }

        right->update_cell(right_child_version, l + offset);
    }
}

}  // namespace std
