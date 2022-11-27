#include <fstream>
#include <iostream>
#include <memory>

#include "SA.h"
#include "output.h"
#include "parse.h"

using namespace std;

int main(int argc, char *argv[]) {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    if (argc != 6) {
        cout << "******************************** [Error] "
                "*******************************"
             << endl;
        cout << "*   [Usage] ./hw3 <input hardblocks>.hardblocks <input "
                "nets>.nets <input pl>.pl <output>.floorplan"
                "<dead space ratio>   *"
             << endl;
        cout << "******************************** [Error] "
                "*******************************"
             << endl;
        return 1;
    }

    fstream hardblock(argv[1], std::fstream::in);
    fstream net(argv[2], std::fstream::in);
    fstream pl(argv[3], std::fstream::in);
    fstream out(argv[4], std::fstream::out);
    fp drc = stof(argv[5]);

    cout << "************* [STATUS] *************" << endl;
    cout << "Start parsing" << endl;
    auto sa_data = parse(hardblock, net, pl, drc);
    cout << "End parsing" << endl;

    SA sa(*sa_data);
    cout << "************* [STATUS] *************" << endl;
    auto result = sa.solve();

    result.output(out);

    hardblock.close();
    net.close();
    pl.close();
    out.close();
    return 0;
}
