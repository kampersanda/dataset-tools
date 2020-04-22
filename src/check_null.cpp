#include <algorithm>
#include <fstream>
#include <iostream>

#include "cmdline.h"

int main(int argc, char* argv[]) {
    std::ios::sync_with_stdio(false);

    cmdline::parser p;
    p.add<std::string>("input_fn", 'i', "input file name", true);
    p.parse_check(argc, argv);

    auto input_fn = p.get<std::string>("input_fn");

    std::ifstream ifs(input_fn);
    if (!ifs) {
        std::cerr << "open error: " << input_fn << '\n';
        return 1;
    }

    char c;
    while (ifs.get(c)) {
        if (c == '\0') {
            std::cout << "NULL character is contained!!" << std::endl;
            return 0;
        }
    }

    std::cout << "NULL character is NOT contained" << std::endl;
    return 0;
}
