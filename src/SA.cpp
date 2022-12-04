#include "SA.h"

#include <assert.h>

#include <cmath>
#include <limits>
#include <random>
#include <stack>
#include <stdexcept>

#include "cell.h"
#include "perturb.h"
#include "type.h"

namespace std {

const fp TIME_LIMIT = 550.0;

void SA::get_initial_solution(vector<intg> &s) {
    intg now_width = 0;
    intg now_height = 0;

    intg col_count = 0;
    intg row_count = 0;
    intg horizon_count = 0;

    for (intg i = 0; i < num_cell; ++i) {
        Cell &c = data.cell_array[i];
        now_width += c.width;
        if (now_width > outline_limit) {
            row_count++;
            if (row_count >= 2) {
                s.emplace_back(static_cast<intg>(SpecialCell::kHorizontal));
                row_count = 1;
                horizon_count++;
            }

            now_width = c.width;
            col_count = 0;
        }

        s.emplace_back(i);

        col_count++;

        if (col_count >= 2) {
            s.emplace_back(static_cast<intg>(SpecialCell::kVertical));
            col_count = 1;
            horizon_count++;
        }
    }

    // add the outside horizontal cut
    for (int i = horizon_count; i < num_cell - 1; ++i) {
        s.emplace_back(static_cast<intg>(SpecialCell::kHorizontal));
    }
}

Cell *SA::build_tree(vector<intg> &e) {
    stack<Cell *> st;
    intg sp_counter = 0;
    intg counter = 0;
    // cout << "============================================" << endl;
    // for (const auto &n : e) {
    //     cout << n << ", ";
    // }
    // cout << endl;

    for (const auto &n : e) {
        counter++;
        if (!is_special_node(n)) {
            // hardblock(cell)
            st.push(&(data.cell_array[n]));
        } else {
            Cell *right_child = st.top();
            st.pop();
            Cell *left_child = st.top();
            st.pop();

            // deal with it
            Cell *special_cell = special_node[sp_counter];
            SpecialCell type;
            if (n == -1) {
                type = SpecialCell::kVertical;
            } else {
                type = SpecialCell::kHorizontal;
            }
            special_cell->type = type;
            special_cell->dp(right_child, left_child);
            st.push(special_cell);
            sp_counter++;
        }
    }
    Cell *ret = st.top();
    st.pop();
    assert(st.size() == 0);
    return ret;
}

intg SA::calculate_wirelength() {
    // asdf
    intg answer = 0;
    for (auto &n : data.net_array) {
        answer += n.hpwl();
    }
    return answer;
}

intg SA::calculate_cost(vector<intg> &e, bool outline_driven) {
    Cell *root = build_tree(e);
    intg min_area = numeric_limits<intg>::max();
    intg exceed_area = 0;

    intg version = 0;
    intg answer_version = -1;
    for (const auto &cr : root->record) {
        exceed_area = 0;
        if (cr.height > outline_limit && cr.width > outline_limit) {
            exceed_area = cr.height * cr.width - outline_limit * outline_limit;
        } else if (cr.height > outline_limit) {
            exceed_area = (cr.height - outline_limit) * outline_limit;
        } else if (cr.width > outline_limit) {
            exceed_area = outline_limit * (cr.width - outline_limit);
        } else {
            exceed_area = 0;
        }

        if (min_area > exceed_area) {
            min_area = exceed_area;
            answer_version = version;
        }

        version++;
    }

    root->update_cell(answer_version, Loc(0, 0));

    if (outline_driven)
        return min_area;

    return min_area * 20 + calculate_wirelength();
}


vector<intg> SA::perturb(vector<intg> &e, intg move_type) {
    // asdf
    PerturbCandidate result;
    vector<intg> s = e;
    switch (move_type) {
    case 0:
        result = swap_cell(s, generator);
        swap(s[result.i], s[result.j]);
        break;
    case 1:
        inverse_cut(s, generator);
        break;
    case 2:
        result = swap_cell_and_cut(s, generator);
        swap(s[result.i], s[result.j]);
        break;
    default:
        throw std::logic_error("Unknown perturb type");
        break;
    }
    return s;
}

intg SA::start_sa(vector<intg> &s, SA_setting &setting, bool outline_driven) {
    intg MT, M, uphill;
    MT = M = uphill = 0;

    intg reject = 0;
    intg N = setting.k * num_cell;

    vector<intg> best, E, NE;
    best = E = s;

    intg best_cost = 0, cost = 0;

    best_cost = cost = calculate_cost(best, outline_driven);

    fp t0 = setting.t0;
    intg epoch = 0;
    if (best_cost == 0)
        goto END;

    setting.iter = 0;
    do {
        do {
            if (!outline_driven) {
                auto now = std::chrono::high_resolution_clock::now();
                double tmp_duration =
                    std::chrono::duration<double, std::ratio<1, 1>>(
                        now - global_start)
                        .count();
                if (tmp_duration > TIME_LIMIT) {
                    break;
                }
            }
            MT = uphill = reject = 0;
            setting.avg_improve = 0;
            setting.avg_up_hill_cost = 0;
            do {
                intg move_type = get_move_type(generator);
                NE = perturb(E, move_type);

                MT += 1;

                intg diff = 0;
                for (int i = 0; i < E.size(); ++i) {
                    if (E[i] != NE[i])
                        diff++;
                }
                // cout << "Diff " << diff << ", with type " << move_type <<
                // endl;
                intg new_cost = calculate_cost(NE, outline_driven);
                intg deltaCost = new_cost - cost;
                if (deltaCost < 0 ||
                    get_accept_prob(generator) < exp(-1 * deltaCost / t0)) {
                    if (deltaCost > 0) {
                        uphill += 1;
                        setting.avg_up_hill_cost += deltaCost;
                        // setting.avg_up_hill_cost += new_cost;
                    } else {
                        setting.avg_improve += deltaCost;
                    }


                    E = NE;
                    cost = new_cost;

                    if (cost < best_cost) {
                        best = E;
                        best_cost = cost;
                        if (best_cost == 0)
                            goto END;
                    }
                } else {
                    reject += 1;
                }
            } while (uphill <= N && MT <= 2 * N);
            cout << "Epoch " << epoch++ << " with cost: " << best_cost
                 << " and temperature: " << t0 << endl;
            // cout << "Uphill/reject: " << uphill << " / " << reject << endl;
            // cout << "N, MT: " << N << ", " << MT << endl;

            if (uphill != 0)
                setting.avg_up_hill_cost /= uphill;
            setting.avg_improve /= (MT - uphill - reject);
            setting.avg_improve *= -1;
            t0 = sa_scheduling(t0, setting, outline_driven);
        } while (reject / MT <= 0.95 && t0 >= setting.c);
        cout << "reject/MT = " << reject << "/" << MT << endl;
        cout << "t0 = " << t0 << endl;
    } while (outline_driven);

END:
    s = best;
    calculate_cost(s, outline_driven);
    return calculate_wirelength();
}

fp SA::sa_scheduling(fp t0, SA_setting &setting, bool outline_driven) {
    if (outline_driven)
        return t0 * setting.r;


    setting.iter++;

    if (setting.iter < setting.threshold)
        return setting.high_t;
    else if (setting.iter == setting.threshold)
        return setting.t0;
    // else if (setting.iter < setting.local_search_threshold)
    //     return t0 * setting.r;
    // else
    //     return t0 * 0.999;
    else
        return t0 * setting.r;

    // if (setting.iter == 0) {
    //     setting.fsa_t0 = setting.t0 * setting.r;
    //     setting.iter++;
    //     return t0 * setting.r;
    //     // setting.fsa_t0 = max(setting.avg_up_hill_cost, setting.t0 *
    //     // setting.r); setting.iter++; return setting.fsa_t0;
    // } else if (setting.iter < setting.fsa_k) {
    //     return (setting.fsa_t0 * setting.avg_improve) /
    //            (static_cast<fp>(setting.iter++) * setting.fsa_c);
    // } else {
    //     return (setting.fsa_t0 * setting.avg_improve) /
    //            (static_cast<fp>(setting.iter++));
    // }
    return t0 * setting.r;
}  // namespace std

intg SA::get_move_type(mt19937 &gen) {
    uniform_real_distribution<fp> gp_noise(0.0, 1.0);
    fp result = gp_noise(gen);

    // if (static_cast<intg>(result) == 1) {
    //     return 0;
    // } else {
    //     return 2;
    // }
    // return static_cast<intg>((result * 3.0));
    return 0;
}

fp SA::get_accept_prob(mt19937 &gen) {
    uniform_real_distribution<fp> gp_noise(0.0, 1.0);
    return gp_noise(gen);
}

}  // namespace std
