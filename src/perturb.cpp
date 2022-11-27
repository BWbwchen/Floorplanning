#include "perturb.h"

#include "cell.h"


namespace std {

PerturbCandidate swap_cell(vector<intg> &e, mt19937 &gen) {
    vector<intg> cell_idx;
    intg idx = 0;
    for (auto &ee : e) {
        if (!is_special_node(ee))
            cell_idx.emplace_back(idx);
        idx++;
    }

    uniform_real_distribution<fp> gp_noise(0.0, cell_idx.size());
    intg pos1 = static_cast<intg>(gp_noise(gen));
    intg pos2 = pos1;

    while (pos1 == pos2) {
        pos2 = static_cast<intg>(gp_noise(gen));
    }

    return PerturbCandidate(cell_idx[pos1], cell_idx[pos2]);
}

PerturbCandidate inverse_cut(vector<intg> &e, mt19937 &gen) {
    vector<intg> cell_idx;
    intg c_idx = 0;
    for (auto &ee : e) {
        if (is_special_node(ee))
            cell_idx.emplace_back(c_idx);
        c_idx++;
    }

    uniform_real_distribution<fp> gp_noise(0.0, cell_idx.size());
    intg idx = cell_idx[static_cast<intg>(gp_noise(gen))];

    while (idx < e.size() && is_special_node(e[idx])) {
        if (e[idx] == static_cast<intg>(SpecialCell::kVertical))
            e[idx] = static_cast<intg>(SpecialCell::kHorizontal);
        else
            e[idx] = static_cast<intg>(SpecialCell::kVertical);
        idx++;
    }
    return {};
}

bool is_ballot(const vector<intg> &e, intg e_idx) {
    // The number of operands is larger than the number of operators.
    if (is_special_node(e[e_idx + 1])) {
        intg N = 0;
        for (int i = 0; i <= e_idx + 1; ++i) {
            if (is_special_node(e[i]))
                N++;
        }

        return (2 * N) < (e_idx + 1);
    }
    return true;
}

bool is_normality(const vector<intg> &e, intg e_idx) {
    // No consecutive operators of the same type in the string.
    if (is_special_node(e[e_idx])) {
        return ((e_idx + 2) < e.size()) && (e[e_idx] != e[e_idx + 2]);
    } else {
        return (e_idx > 0) && (e[e_idx + 1] != e[e_idx - 1]);
    }
    return true;
}

PerturbCandidate swap_cell_and_cut(vector<intg> &e, mt19937 &gen) {
    vector<intg> cell_idx;
    for (intg i = 0; i < e.size() - 1; ++i) {
        if (is_special_node(e[i]) && !is_special_node(e[i + 1])) {
            cell_idx.emplace_back(i);
        } else if (!is_special_node(e[i]) && is_special_node(e[i + 1])) {
            cell_idx.emplace_back(i);
        }
    }

    uniform_real_distribution<fp> gp_noise(0.0, cell_idx.size());
    intg idx = -1;
    do {
        idx = static_cast<intg>(gp_noise(gen));
        // TODO: violation count?
    } while (!is_ballot(e, cell_idx[idx]) || !is_normality(e, cell_idx[idx]));

    return PerturbCandidate(cell_idx[idx], cell_idx[idx] + 1);
}



}  // namespace std
