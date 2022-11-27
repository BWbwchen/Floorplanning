#ifndef PERTURB_H_
#define PERTURB_H_

#include <random>
#include <vector>

#include "type.h"

namespace std {

class PerturbCandidate {
public:
    intg i;
    intg j;

public:
    PerturbCandidate() {}
    PerturbCandidate(intg _i, intg _j) : i(_i), j(_j) {}
};

PerturbCandidate swap_cell(vector<intg> &e, mt19937 &gen);
PerturbCandidate inverse_cut(vector<intg> &e, mt19937 &gen);
PerturbCandidate swap_cell_and_cut(vector<intg> &e, mt19937 &gen);


}  // namespace std


#endif  // PERTURB_H_
