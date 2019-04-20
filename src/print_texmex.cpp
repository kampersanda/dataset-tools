#include <algorithm>
#include <fstream>
#include <iostream>

#include "cmdline.h"

template <class T>
int print(const std::string& input_fn, size_t num) {
  std::ifstream ifs(input_fn);
  if (!ifs) {
    std::cerr << "open error: " << input_fn << '\n';
    return 1;
  }

  for (size_t i = 0; i < num; ++i) {
    uint32_t dim;
    ifs.read(reinterpret_cast<char*>(&dim), sizeof(dim));
    if (ifs.eof()) {
      break;
    }
    std::cout << dim << '\t';
    for (uint32_t j = 0; j < dim; ++j) {
      T v;
      ifs.read(reinterpret_cast<char*>(&v), sizeof(v));
      if constexpr (std::is_same_v<uint8_t, T>) {
        std::cout << float(v) << ' ';
      } else {
        std::cout << v << ' ';
      }
    }
    std::cout << '\n';
  }

  return 0;
}

int main(int argc, char** argv) {
  std::ios::sync_with_stdio(false);

  cmdline::parser p;
  p.add<std::string>("input_fn", 'i', "input file name", true);
  p.add<std::string>("format", 'f', "fvecs | ivecs | bvecs", true);
  p.add<size_t>("num", 'n', "number of sketches printed", false, 5);
  p.parse_check(argc, argv);

  auto input_fn = p.get<std::string>("input_fn");
  auto format = p.get<std::string>("format");
  auto num = p.get<size_t>("num");

  if (format == "fvecs") {
    return print<float>(input_fn, num);
  }
  if (format == "ivecs") {
    return print<uint32_t>(input_fn, num);
  }
  if (format == "bvecs") {
    return print<uint8_t>(input_fn, num);
  }

  return 1;
}