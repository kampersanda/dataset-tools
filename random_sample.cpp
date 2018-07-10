
#include <fstream>
#include <iostream>
#include <random>

#include "cmdline.h"

int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("input_fn", 'i', "input file name", true);
  p.add<size_t>("num", 'n', "# of samples", true);
  p.parse_check(argc, argv);

  auto input_fn = p.get<std::string>("input_fn");
  auto num = p.get<size_t>("num");

  std::ifstream ifs(input_fn);
  if (!ifs) {
    std::cerr << "open error: " << input_fn << '\n';
    return 1;
  }

  std::vector<std::string> data;
  for (std::string line; std::getline(ifs, line);) {
    data.push_back(line);
  }

  std::random_device rd;
  for (size_t i = 0; i < num; ++i) {
    std::cout << data[rd() % data.size()] << '\n';
  }

  return 0;
}
