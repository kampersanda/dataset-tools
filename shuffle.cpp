
#include <fstream>
#include <iostream>
#include <random>

#include "cmdline.h"

int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("data_fn", 'd', "input file name of data vectors", true);
  p.parse_check(argc, argv);

  auto data_fn = p.get<std::string>("data_fn");

  std::ifstream ifs(data_fn);
  if (!ifs) {
    std::cerr << "open error: " << data_fn << '\n';
    return 1;
  }

  std::vector<std::string> data;
  for (std::string line; std::getline(ifs, line);) {
    data.push_back(line);
  }

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::shuffle(data.begin(), data.end(), engine);

  for (const auto& v : data) {
    std::cout << v << '\n';
  }

  return 0;
}
