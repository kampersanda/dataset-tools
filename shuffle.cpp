
#include <fstream>
#include <iostream>
#include <random>

#include "cmdline.h"

void shuffle(std::istream& is, std::ostream& os) {
  std::vector<std::string> data;
  for (std::string line; std::getline(is, line);) {
    data.push_back(line);
  }

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::shuffle(data.begin(), data.end(), engine);

  for (const auto& v : data) {
    os << v << '\n';
  }
}

int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("input_fn", 'i', "input file name", true);
  p.add<std::string>("output_fn", 'o', "output file name", false, "");
  p.parse_check(argc, argv);

  auto input_fn = p.get<std::string>("input_fn");
  auto output_fn = p.get<std::string>("output_fn");

  if (input_fn == output_fn) {
    std::cerr << "error: input_fn == output_fn\n";
    return 1;
  }

  std::ifstream ifs(input_fn);
  if (!ifs) {
    std::cerr << "open error: " << input_fn << '\n';
    return 1;
  }

  if (output_fn.empty()) {
    shuffle(ifs, std::cout);
  } else {
    std::ofstream ofs(output_fn);
    if (!ofs) {
      std::cerr << "open error: " << output_fn << '\n';
      return 1;
    }
    shuffle(ifs, ofs);
  }

  return 0;
}
