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
  int pid;
  int start;
  explicit AOC_Input(std::string const &s){
    std::sscanf(s.c_str(), "Player %d starting position: %d", &pid, &start);
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

class Dice{
public:
  auto roll() {
    auto s = state_;
    ++state_;
    if (state_ > 100) state_ = 1;
    return s;
  };
private:
  long state_ = 1;
};

AOC_Output part_1(std::vector<AOC_Input> const &v){
  int p_pos[2] = {v[0].start, v[1].start};
  unsigned long long p_score[2] = {0, 0};

  Dice d;
  auto rolls = 0ull;
  auto pid = 0;
  while (p_score[0] < 1000 && p_score[1] < 1000){
    auto forward = d.roll() + d.roll() + d.roll();
    p_pos[pid] = p_pos[pid] + forward;
    while (p_pos[pid] > 10) //Govnocode!!!! Wooooohooo!
      p_pos[pid] -= 10;

    p_score[pid] += p_pos[pid];
    pid = (pid + 1) % 2;
    ++rolls;
  }

  return static_cast<long>((p_score[0] >= 1000) ? p_score[1] * rolls * 3ull : p_score[0] * rolls * 3ull);
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  return 0;
}

#include "../exec.hpp"