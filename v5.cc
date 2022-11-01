#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <charconv>
#include <algorithm>

void Usage(std::string const &program) {
  std::cerr << "Usage: " << program << " [INPUT [OUTPUT]]\n";
  std::cerr << "INPUT: input file. Default: stdin\n"
               "OUTPUT: output file. Default: stdout\n";
}

struct Token {
  char const *first;
  char const *last;

  explicit Token(const char *first, const char *last)
      : first(first), last(last) {}
};

template<typename Pred>
auto Split(std::string const &str, Pred pred) {
  std::vector<Token> tokens;
  auto first = str.begin();
  auto last = str.end();
  while (first < last) {
    auto it = std::find_if(first, last, pred);
    tokens.emplace_back(&*first, &*last);
    first = it + 1;
  }
  return tokens;
}

std::string ReadAll(std::istream &istream) {
  constexpr long BUFFER_SIZE = 4096;
  std::string result;
  auto cur_size = result.size();
  while (true) {
    result.resize(cur_size + BUFFER_SIZE);
    auto n = istream.read(&result[cur_size], BUFFER_SIZE).gcount();
    if (n == BUFFER_SIZE)
      cur_size = result.size();
    else {
      result.resize(cur_size + n);
      break;
    }
  }
  return result;
}

int main(int argc, const char **argv) {
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
  auto input = ReadAll(ifs);
  auto tokens = Split(input, [](auto c) { return std::isspace(c); });
  for (auto &token: tokens) {
    if (token.first == token.last) continue;
    int64_t val;
    auto [ptr, ec] {std::from_chars(token.first, token.last, val)};
    if (ptr == token.last) break;
    xs.push_back(val);
  }

  std::sort(xs.begin(), xs.end());
  for (auto x: xs) {
    auto s = std::to_string(x) + "\n";
    ofs.write(s.c_str(), s.size());
  }

  return 0;
}
