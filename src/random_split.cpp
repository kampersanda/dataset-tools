
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
  std::ios::sync_with_stdio(false);

  cmdline::parser p;
  p.add<std::string>("input_fn", 'i', "input file name", true);
  p.add<std::string>("A_fn", 'A', "output file name of one side", false, "");
  p.add<std::string>("B_fn", 'B', "output file name of another side", false, "");
  p.add<size_t>("num", 'n', "# of entries of A", true);
  p.parse_check(argc, argv);

  auto input_fn = p.get<std::string>("input_fn");
  auto A_fn = p.get<std::string>("A_fn");
  auto B_fn = p.get<std::string>("B_fn");
  auto num = p.get<size_t>("num");

  if ((input_fn == A_fn) or (input_fn == B_fn) or (A_fn == B_fn)) {
    std::cerr << "error: filenames are same\n";
    return 1;
  }

  std::ifstream ifs(input_fn);
  if (!ifs) {
    std::cerr << "open error: " << input_fn << '\n';
    return 1;
  }

  std::vector<std::string> data;
  for (std::string line; std::getline(ifs, line);) {
    data.push_back(line);
  }

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::shuffle(data.begin(), data.end(), engine);

  size_t i = 0;

  if (num > data.size()) {
    num = data.size();
  }

  {
    std::ofstream ofs(A_fn);
    if (!ofs) {
      std::cerr << "open error: " << A_fn << '\n';
      return 1;
    }
    for (; i < num; ++i) {
      ofs << data[i] << '\n';
    }
  }

  {
    std::ofstream ofs(B_fn);
    if (!ofs) {
      std::cerr << "open error: " << B_fn << '\n';
      return 1;
    }
    for (; i < data.size(); ++i) {
      ofs << data[i] << '\n';
    }
  }

  return 0;
}
