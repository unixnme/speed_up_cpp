#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

void Usage(std::string const &program) {
  std::cerr << "Usage: " << program << " [INPUT [OUTPUT]]\n";
  std::cerr << "INPUT: input file. Default: stdin\n"
               "OUTPUT: output file. Default: stdout\n";
}

int main(int argc, const char **argv) {
  std::ios_base::sync_with_stdio(false);
  auto Error = [argv](std::string const &msg) {
    std::cerr << msg << "\n";
    Usage(argv[0]);
    exit(EXIT_FAILURE);
  };

  std::string input_file = "/dev/stdin";
  std::string output_file = "/dev/stdout";
  if (argc >= 2) input_file = argv[1];
  if (argc >= 3) output_file = argv[2];

  std::ifstream ifs{input_file};
  if (!ifs) Error("Cannot open " + input_file);
  std::ofstream ofs{output_file};
  if (!ofs) Error("Cannot open " + output_file);

  std::vector<int64_t> xs;
  while (true) {
    int64_t val;
    ifs >> val;
    if (ifs) xs.push_back(val);
    else break;
  }

  std::sort(xs.begin(), xs.end());
  for (auto x: xs) {
    ofs << x << "\n";
  }

  return 0;
}
