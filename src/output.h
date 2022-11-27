#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "cell.h"


namespace std {


class Output {
public:
    vector<Cell> *root;
    intg wl;

public:
    Output(vector<Cell> *r, intg _wl) : root(r), wl(_wl) {}
    void output(fstream &f);
};

}  // namespace std
#endif  // OUTPUT_H_
