
#include <fstream>
#include <iostream>
#include <random>

#include "cmdline.h"

int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("input_fn", 'i', "input file name", true);
  p.add<size_t>("step", 's', "step width for down sample", true);
  p.parse_check(argc, argv);

  auto input_fn = p.get<std::string>("input_fn");
  auto step = p.get<size_t>("step");

  std::ifstream ifs(input_fn);
  if (!ifs) {
    std::cerr << "open error: " << input_fn << '\n';
    return 1;
  }

  size_t i = 0;
  for (std::string line; std::getline(ifs, line);) {
    if (i == 0) {
      std::cout << line << '\n';
    }
    if (++i == step) {
      i = 0;
    }
  }

  return 0;
}
