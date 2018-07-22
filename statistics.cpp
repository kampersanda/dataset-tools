
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>

#include "cmdline.h"

template <bool Terminator>
void statistics(std::istream& is, std::ostream& os) {
  size_t num_strs = 0;
  size_t num_chars = 0;
  size_t min_length = std::numeric_limits<size_t>::max();
  size_t max_length = 0;

  for (std::string line; std::getline(is, line);) {
    ++num_strs;
    auto length = line.size();
    if constexpr (Terminator) {
      ++length;
    }
    num_chars += length;
    min_length = std::min(length, min_length);
    max_length = std::max(length, max_length);
  }

  os << "num_strs:   " << num_strs << '\n';
  os << "num_chars:  " << num_chars << '\n';
  os << "min_length: " << min_length << '\n';
  os << "max_length: " << max_length << '\n';
  os << "ave_length: " << double(num_chars) / num_strs << '\n';
}

int main(int argc, char* argv[]) {
  cmdline::parser p;
  p.add<std::string>("input_fn", 'i', "input file name", true);
  p.add<bool>("terminator", 't', "consider the terminator", false, true);
  p.parse_check(argc, argv);

  auto input_fn = p.get<std::string>("input_fn");
  auto terminator = p.get<bool>("terminator");

  std::ifstream ifs(input_fn);
  if (!ifs) {
    std::cerr << "open error: " << input_fn << '\n';
    return 1;
  }

  if (terminator) {
    statistics<true>(ifs, std::cout);
  } else {
    statistics<false>(ifs, std::cout);
  }

  return 0;
}
