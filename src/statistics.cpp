
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>

#include "cmdline.h"

double zero_emp_entropy(const size_t* freqs, size_t n) {
  double entropy = 0.0;
  for (int i = 0; i < 256; ++i) {
    if (freqs[i] != 0) {
      double nc = double(freqs[i]);
      entropy += (nc / n) * std::log2(n / nc);
    }
  }
  return entropy;
}

template <bool Terminator>
void statistics(std::istream& is, std::ostream& os) {
  size_t num_strs = 0;
  size_t num_chars = 0;
  size_t min_length = std::numeric_limits<size_t>::max();
  size_t max_length = 0;

  size_t char_freqs[256];
  std::fill(char_freqs, char_freqs + 256, 0);

  for (std::string line; std::getline(is, line);) {
    ++num_strs;
    auto length = line.size();
    if constexpr (Terminator) {
      ++length;
    }
    num_chars += length;
    min_length = std::min(length, min_length);
    max_length = std::max(length, max_length);
    for (uint8_t c : line) {
      ++char_freqs[c];
    }
  }

  if constexpr (Terminator) {
    char_freqs[0] += num_strs;
  }
  size_t alph_size = 0;
  for (size_t n : char_freqs) {
    if (n != 0) {
      ++alph_size;
    }
  }

  os << "num_strs:   " << num_strs << '\n';
  os << " - in Mega: " << double(num_strs) / (1000 * 1000) << '\n';
  os << " - in Giga: " << double(num_strs) / (1000 * 1000 * 1000) << '\n';
  os << "num_chars:  " << num_chars << '\n';
  os << " - in Mebi: " << double(num_chars) / (1024 * 1024) << '\n';
  os << " - in Gibi: " << double(num_chars) / (1024 * 1024 * 1024) << '\n';
  os << "min_length: " << min_length << '\n';
  os << "max_length: " << max_length << '\n';
  os << "ave_length: " << double(num_chars) / num_strs << '\n';
  os << "alph_size:  " << alph_size << '\n';
  os << "0_entropy:  " << zero_emp_entropy(char_freqs, num_chars) << '\n';
}

int main(int argc, char* argv[]) {
  std::ios::sync_with_stdio(false);

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
