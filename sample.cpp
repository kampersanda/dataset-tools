
#include <fstream>
#include <iostream>
#include <random>

#include "cmdline.h"

int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("data_fn", 'd', "input file name of data vectors", true);
  p.add<size_t>("samples", 's', "#samples", true);
  p.parse_check(argc, argv);

  auto data_fn = p.get<std::string>("data_fn");
  auto samples = p.get<size_t>("samples");

  std::ifstream ifs(data_fn);
  if (!ifs) {
    std::cerr << "open error: " << data_fn << '\n';
    return 1;
  }

  std::vector<std::string> data;
  for (std::string line; std::getline(ifs, line);) {
    data.push_back(line);
  }

  std::random_device rd;
  for (size_t i = 0; i < samples; ++i) {
    std::cout << data[rd() % data.size()] << '\n';
  }

  return 0;
}
