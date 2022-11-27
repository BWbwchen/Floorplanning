#include "output.h"

#include <fstream>
#include <queue>

namespace std {

void Output::output(fstream &f) {
    f << "Wirelength " << this->wl << endl;

    f << "Blocks" << endl;
    auto &c_array = *root;

    for (int i = 0; i < c_array.size(); ++i) {
        auto &c = c_array[i];
        f << c.name << " " << c.loc.get_x() << " " << c.loc.get_y() << " "
          << (c.rotate ? "1" : "0") << endl;
    }

    return;
}


}  // namespace std
