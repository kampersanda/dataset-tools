
#include <fstream>
#include <iostream>
#include <random>

#include "cmdline.h"

void down_sample(std::istream& is, std::ostream& os, size_t step) {
  size_t i = 0;
  for (std::string line; std::getline(is, line);) {
    if (i == 0) {
      os << line << '\n';
    }
    if (++i == step) {
      i = 0;
    }
  }
}

int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("input_fn", 'i', "input file name", true);
  p.add<std::string>("output_fn", 'o', "output file name", false, "");
  p.add<size_t>("step", 's', "step width for down sample", true);
  p.parse_check(argc, argv);

  auto input_fn = p.get<std::string>("input_fn");
  auto output_fn = p.get<std::string>("output_fn");
  auto step = p.get<size_t>("step");

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
    down_sample(ifs, std::cout, step);
  } else {
    std::ofstream ofs(output_fn);
    if (!ofs) {
      std::cerr << "open error: " << output_fn << '\n';
      return 1;
    }
    down_sample(ifs, ofs, step);
  }

  return 0;
}
