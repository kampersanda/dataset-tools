
#include <fstream>
#include <iostream>
#include <random>

#include "cmdline.h"

const uint32_t DIM = 384;

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

  std::vector<float> vecs;

  {
    std::ifstream ifs(input_fn);
    if (!ifs) {
      std::cerr << "open error: " << input_fn << '\n';
      return 1;
    }

    float buf[DIM] = {};

    while (true) {
      ifs.read(reinterpret_cast<char*>(buf), sizeof(float) * DIM);
      if (ifs.eof()) {
        break;
      }
      std::copy(buf, buf + DIM, std::back_inserter(vecs));
    }
  }

  std::cout << "# of images: " << vecs.size() / DIM << std::endl;

  std::vector<const float*> ptrs;
  ptrs.reserve(vecs.size() / DIM);

  for (size_t i = 0; i < vecs.size(); i += DIM) {
    ptrs.push_back(vecs.data() + i);
  }

  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  std::shuffle(ptrs.begin(), ptrs.end(), engine);

  size_t i = 0;

  if (num > ptrs.size()) {
    num = ptrs.size();
  }

  {
    A_fn += ".fvecs";
    std::ofstream ofs(A_fn);
    if (!ofs) {
      std::cerr << "open error: " << A_fn << '\n';
      return 1;
    }
    for (; i < num; ++i) {
      ofs.write(reinterpret_cast<const char*>(&DIM), sizeof(uint32_t));
      ofs.write(reinterpret_cast<const char*>(ptrs[i]), sizeof(float) * DIM);
    }
  }

  {
    B_fn += ".fvecs";
    std::ofstream ofs(B_fn);
    if (!ofs) {
      std::cerr << "open error: " << B_fn << '\n';
      return 1;
    }
    for (; i < ptrs.size(); ++i) {
      ofs.write(reinterpret_cast<const char*>(&DIM), sizeof(uint32_t));
      ofs.write(reinterpret_cast<const char*>(ptrs[i]), sizeof(float) * DIM);
    }
  }

  return 0;
}
