//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output{
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::string line;
  explicit AOC_Input(std::string s): line(std::move(s)){
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

using world_map = std::vector<std::vector<char>>;

AOC_Output part_1(std::vector<AOC_Input> const &v){
  world_map w(v.size(), std::vector<char>(v.front().line.size(), '.'));

  for (auto i = 0ul; auto const& e : v){
    std::ranges::copy(e.line, w[i].begin());
    ++i;
  }

  world_map wc = w;

  bool is_moving = true;
  auto step = 0;

  while (is_moving){
    is_moving = false;
    for (auto i = 0ul; i < w.size(); ++i)
      for (auto j = 0ul; j < w.front().size(); ++j){
        if (w[i][j] == '>' && w[i][(j + 1) % w.front().size()] == '.') {
          wc[i][(j + 1) % w.front().size()] = '>';
          wc[i][j] = '.';
          is_moving = true;
        }
      }
    w = wc;
    for (auto i = 0ul; i < w.size(); ++i)
      for (auto j = 0ul; j < w.front().size(); ++j){
        if (w[i][j] == 'v' && w[(i + 1) % w.size()][j] == '.') {
          wc[(i + 1) % w.size()][j] = 'v';
          wc[i][j] = '.';
          is_moving = true;
        }
      }
    w = wc;
    ++step;

//    for (auto const &e: w) {
//      std::ranges::copy(e, std::ostream_iterator<char>(std::cout, ""));
//      std::cout << "\n";
//    }
//    std::cout << "\n";
  }

  return step;
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  return 0;
}

#include "../exec.hpp"