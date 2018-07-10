
#include <algorithm>
#include <fstream>
#include <iostream>

#include "cmdline.h"

int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("input_fn", 'i', "input file name", true);
  p.parse_check(argc, argv);

  auto input_fn = p.get<std::string>("input_fn");

  std::ifstream ifs(input_fn);
  if (!ifs) {
    std::cerr << "open error: " << input_fn << '\n';
    return 1;
  }

  std::vector<std::string> data;
  for (std::string line; std::getline(ifs, line);) {
    data.push_back(line);
  }

  std::sort(data.begin(), data.end());
  data.erase(std::unique(data.begin(), data.end()), data.end());

  for (const auto& v : data) {
    std::cout << v << '\n';
  }

  return 0;
}
