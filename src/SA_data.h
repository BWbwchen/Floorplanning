#ifndef SA_DATA_H_
#define SA_DATA_H_

#include <memory>
#include <vector>

#include "cell.h"
#include "net.h"

namespace std {

class SA_data {
public:
    vector<Cell> cell_array;
    vector<Net> net_array;

    // dead space ratio
    fp dsr;

public:
    SA_data(vector<Cell> vc, vector<Net> vn, fp _dsr)
        : cell_array(move(vc)), net_array(move(vn)), dsr(_dsr) {}
};



}  // namespace std


#endif  // SA_DATA_H_
