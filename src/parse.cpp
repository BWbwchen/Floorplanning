#include "parse.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace std {

vector<Cell> parse_hardblock(fstream &fs);
vector<Net> parse_net(fstream &net, fstream &pl);

unordered_map<string, Pin *> name_to_pin_ptr;

SA_data *parse(fstream &hardblock, fstream &net, fstream &pl, fp drc) {
    name_to_pin_ptr.clear();
    return new SA_data(parse_hardblock(hardblock), parse_net(net, pl), drc);
}

vector<Cell> parse_hardblock(fstream &fs) {
    // asdf
    vector<Cell> ret;
    string tmp;

    intg num_hb;
    fs >> tmp >> tmp >> num_hb;
    cout << "Num of hardblock: " << num_hb << endl;
    ret.reserve(num_hb);

    intg num_terminal;
    fs >> tmp >> tmp >> num_terminal;
    cout << "Num of terminal: " << num_hb << endl;

    string hb_name;
    // sb0 hardrectilinear 4 (0, 0) (0, 33) (43, 33) (43, 0)
    for (int i = 0; i < num_hb; ++i) {
        fs >> hb_name >> tmp >> tmp;

        // (0, 0) (0, 33) (43, 33) (43, 0)
        string buff;
        getline(fs, buff);

        int x1, x2, x3, x4;
        int y1, y2, y3, y4;

        sscanf(buff.c_str(), " (%d, %d) (%d, %d) (%d, %d) (%d, %d)", &x1, &y1,
               &x2, &y2, &x3, &y3, &x4, &y4);
        // printf("(%d, %d) (%d, %d) (%d, %d) (%d, %d)\n", x1, y1, x2, y2, x3,
        // y3, x4, y4);
        intg width = x3 - x1;
        intg height = y2 - y1;

        Pin *p = new Pin(hb_name, Loc(width / 2, height / 2));

        ret.emplace_back(Cell(hb_name, width, height, p));
        name_to_pin_ptr[hb_name] = p;
    }

    return ret;
}

vector<Net> parse_net(fstream &net, fstream &pl) {
    // read pl first
    string pin_name;
    intg x, y;
    while (pl >> pin_name) {
        pl >> x >> y;
        name_to_pin_ptr[pin_name] = new Pin(pin_name, Loc(x, y));
    }
    cout << "Parse pl finished." << endl;

    // read net
    string tmp;
    string net_name;
    intg num_net;
    intg num_pin;

    net >> tmp >> tmp >> num_net;
    cout << "Num of nets: " << num_net << endl;
    net >> tmp >> tmp >> num_pin;
    cout << "Num of pins: " << num_pin << endl;

    vector<Net> vn;
    vector<Pin *> vp;
    for (int i = 0; i < num_net; ++i) {
        vp.clear();
        int np;
        net >> tmp >> tmp >> np;
        for (int j = 0; j < np; ++j) {
            net >> pin_name;
            vp.emplace_back(name_to_pin_ptr[pin_name]);
        }
        vn.emplace_back(Net(vp));
    }

    return vn;
}

}  // namespace std
