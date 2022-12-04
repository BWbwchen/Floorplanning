#include <chrono>
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
    auto parsing_start = std::chrono::high_resolution_clock::now();
    auto sa_data = parse(hardblock, net, pl, drc);
    auto parsing_end = std::chrono::high_resolution_clock::now();
    double parsing_duration = std::chrono::duration<double, std::ratio<1, 1>>(
                                  parsing_end - parsing_start)
                                  .count();
    cout << "Parsing time: " << parsing_duration << endl;
    cout << "************* [STATUS] *************" << endl;

    SA sa(*sa_data);
    auto result = sa.solve();

    auto output_start = std::chrono::high_resolution_clock::now();
    result.output(out);
    auto output_end = std::chrono::high_resolution_clock::now();
    double output_duration = std::chrono::duration<double, std::ratio<1, 1>>(
                                 output_end - output_start)
                                 .count();
    cout << "IO Time: " << output_duration + parsing_duration << endl;

    hardblock.close();
    net.close();
    pl.close();
    out.close();
    return 0;
}
