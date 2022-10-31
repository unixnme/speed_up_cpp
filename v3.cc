#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

void Usage(std::string const& program) {
  std::cerr << "Usage: " << program << " [INPUT [OUTPUT]]\n";
  std::cerr << "INPUT: input file. Default: stdin\n"
               "OUTPUT: output file. Default: stdout\n";
}

template<typename Pred>
std::vector<std::string> Split(std::string const& str, Pred pred) {
  std::vector<std::string> tokens;
  auto first = str.begin();
  auto last = str.end();
  while (first < last) {
    auto it = std::find_if(first, last, pred);
    tokens.push_back(std::string{first, it});
    first = it + 1;
  }
  return tokens;
}

int main(int argc, const char** argv) {
  auto Error = [argv] (std::string const& msg){
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
  std::ofstream  ofs{output_file};
  if (!ofs) Error("Cannot open " + output_file);

  std::vector<int64_t> xs;
  std::string input(std::istreambuf_iterator<char>{ifs}, {});
  auto tokens = Split(input, [](auto c) { return std::isspace(c); });
  for (auto &token : tokens) {
    if (token.empty()) continue;
    auto val = std::stoll(token);
    xs.push_back(val);
  }

  std::sort(xs.begin(), xs.end());
  for (auto x : xs) {
    auto s = std::to_string(x) + "\n";
    ofs.write(s.c_str(), s.size());
  }

  return 0;
}
