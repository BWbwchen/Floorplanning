#ifndef SA_H_
#define SA_H_

#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "SA_data.h"
#include "cell.h"
#include "output.h"

const intg random_seed = 1000;

namespace std {

class SA {
public:
    mt19937 generator;

    SA_data data;
    vector<intg> sol;

    intg num_cell;
    intg outline_limit;

    class SA_setting {
    public:
        intg k;  // how many number of move in each loop (in terms of times of
                 // num_cells )
        fp c;    // end temperature
        fp r;    // decrease constant
        fp t0;   // initial temperature
    public:
        SA_setting() = default;
    };

    SA_setting sa_setting;

    vector<Cell *> special_node;

    void get_initial_solution(vector<intg> &s);
    intg start_sa(vector<intg> &s, SA_setting &setting);
    intg calculate_cost(vector<intg> &e);
    intg get_move_type(mt19937 &gen);
    fp get_accept_prob(mt19937 &gen);
    vector<intg> perturb(vector<intg> &s, intg move_type);
    fp sa_scheduling(fp t0, SA_setting &setting);
    Cell *build_tree(vector<intg> &e);
    intg calculate_wirelength();

public:
    SA(SA_data _d) : data(move(_d)) {
        num_cell = data.cell_array.size();
        intg total_block_area = 0;
        for (int i = 0; i < num_cell; ++i) {
            total_block_area += data.cell_array[i].get_area();
        }
        outline_limit = static_cast<intg>(
            sqrt(static_cast<fp>(total_block_area) * (1 + data.dsr)));

        cout << "Num Cells: " << num_cell << endl;
        cout << "Num Nets: " << data.net_array.size() << endl;
        cout << "Outline Limits: " << outline_limit << endl;

        for (int i = 0; i < num_cell; ++i) {
            special_node.emplace_back(
                new Cell(SpecialCell::kVertical, nullptr, nullptr));
        }

        generator.seed(random_seed);
    }
    Output solve() {
        get_initial_solution(sol);

        sa_setting.c = 1;
        sa_setting.r = 0.95;
        sa_setting.k = 5;
        sa_setting.t0 = 1000.0;

        intg wl = start_sa(sol, sa_setting);

        return Output(&(data.cell_array), wl);
    }
};

}  // namespace std


#endif  // SA_H_
