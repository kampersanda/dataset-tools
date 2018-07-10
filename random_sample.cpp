
#include <fstream>
#include <iostream>
#include <random>

#include "cmdline.h"

void random_sample(std::istream& is, std::ostream& os, size_t num) {
  std::vector<std::string> data;
  for (std::string line; std::getline(is, line);) {
    data.push_back(line);
  }

  std::random_device rd;
  for (size_t i = 0; i < num; ++i) {
    os << data[rd() % data.size()] << '\n';
  }
}

int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("input_fn", 'i', "input file name", true);
  p.add<std::string>("output_fn", 'o', "output file name", false, "");
  p.add<size_t>("num", 'n', "# of samples", true);
  p.parse_check(argc, argv);

  auto input_fn = p.get<std::string>("input_fn");
  auto output_fn = p.get<std::string>("output_fn");
  auto num = p.get<size_t>("num");

  std::ifstream ifs(input_fn);
  if (!ifs) {
    std::cerr << "open error: " << input_fn << '\n';
    return 1;
  }

  if (output_fn.empty()) {
    random_sample(ifs, std::cout, num);
  } else {
    std::ofstream ofs(output_fn);
    if (!ofs) {
      std::cerr << "open error: " << output_fn << '\n';
      return 1;
    }
    random_sample(ifs, ofs, num);
  }

  return 0;
}
