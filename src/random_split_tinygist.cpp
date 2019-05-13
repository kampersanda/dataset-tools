
#include <fstream>
#include <iostream>
#include <random>
#include <set>

#include "cmdline.h"

const uint32_t DIM = 384;

size_t get_file_size(char const* filename) {
  std::ifstream is(filename, std::ios::binary | std::ios::ate);
  if (!is.good()) {
    throw std::runtime_error(
        "Error in opening binary "
        "file.");
  }
  size_t bytes = (size_t)is.tellg();
  is.close();
  return bytes;
}

std::vector<size_t> random_choice(size_t n, size_t max) {
  std::set<size_t> dup;

  std::random_device random_device;
  std::mt19937 engine{random_device()};
  std::uniform_int_distribution<size_t> dist(0, max);

  while (dup.size() < n) {
    dup.insert(dist(engine));
  }

  std::vector<size_t> ret;
  for (auto v : dup) {
    ret.push_back(v);
  }

  return ret;
}

int main(int argc, char* argv[]) {
  std::ios::sync_with_stdio(false);

  cmdline::parser p;
  p.add<std::string>("input_fn", 'i', "input file name", true);
  p.add<std::string>("A_fn", 'A', "output file name of one side (in fvecs format)", false, "");
  p.add<std::string>("B_fn", 'B', "output file name of another side (in fvecs format)", false, "");
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

  size_t filesize = get_file_size(input_fn.c_str());
  if (filesize % (DIM * sizeof(float)) != 0) {
    std::cerr << "error: filesize is strange\n";
    return 1;
  }

  size_t num_images = filesize / (DIM * sizeof(float));
  std::cout << "# of images: " << num_images << std::endl;

  auto A = random_choice(num, num_images - 1);
  A.push_back(num_images);

  {
    std::ifstream ifs(input_fn);
    if (!ifs) {
      std::cerr << "open error: " << input_fn << '\n';
      return 1;
    }

    A_fn += ".fvecs";
    std::ofstream ofsA(A_fn);
    if (!ofsA) {
      std::cerr << "open error: " << A_fn << '\n';
      return 1;
    }

    B_fn += ".fvecs";
    std::ofstream ofsB(B_fn);
    if (!ofsB) {
      std::cerr << "open error: " << B_fn << '\n';
      return 1;
    }

    float buf[DIM] = {};

    size_t j = 0;
    const size_t bytes = sizeof(float) * DIM;

    size_t print_point = 0;

    for (size_t i = 0; i < num_images; i++) {
      ifs.read(reinterpret_cast<char*>(buf), bytes);
      if (A[j] == i) {
        ofsA.write(reinterpret_cast<const char*>(&DIM), sizeof(uint32_t));
        ofsA.write(reinterpret_cast<const char*>(buf), bytes);
        ++j;
      } else {
        ofsB.write(reinterpret_cast<const char*>(&DIM), sizeof(uint32_t));
        ofsB.write(reinterpret_cast<const char*>(buf), bytes);
      }
      if (i == print_point) {
        std::cout << i << " processed" << std::endl;
        print_point += 1000000;
      }
    }
  }

  return 0;
}
