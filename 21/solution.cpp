//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output {
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  int pid;
  int start;
  explicit AOC_Input(std::string const &s) { std::sscanf(s.c_str(), "Player %d starting position: %d", &pid, &start); }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) { return AOC_Input(line); }

class Dice {
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

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  int p_pos[2] = {v[0].start, v[1].start};
  unsigned long long p_score[2] = {0, 0};

  Dice d;
  auto rolls = 0ull;
  auto pid = 0;
  while (p_score[0] < 1000 && p_score[1] < 1000) {
    auto forward = d.roll() + d.roll() + d.roll();
    p_pos[pid] = p_pos[pid] + forward;
    while (p_pos[pid] > 10)//Govnocode!!!! Wooooohooo!
      p_pos[pid] -= 10;

    p_score[pid] += p_pos[pid];
    pid = (pid + 1) % 2;
    ++rolls;
  }

  return static_cast<long>((p_score[0] >= 1000) ? p_score[1] * rolls * 3ull : p_score[0] * rolls * 3ull);
}

std::optional<std::pair<unsigned long long, unsigned long long>> wins_temp[22][22][11][11][2];

std::pair<unsigned long long, unsigned long long> game_step(std::array<long, 2> scores, std::array<long, 2> pos, int dice, int pid) {
  pos[pid] += dice;
  if (pos[pid] > 10) pos[pid] -= 10;

  scores[pid] += pos[pid];
  if (scores[pid] >= 21) { return (pid == 0) ? std::pair{1ull, 0ull} : std::pair{0ull, 1ull}; }

  auto npid = (pid + 1) % 2;
  if (!wins_temp[scores[0]][scores[1]][pos[0]][pos[1]][npid]) {
    unsigned long long p1_wins = 0, p2_wins = 0;
    for (auto i: {1, 2, 3})
      for (auto j: {1, 2, 3})
        for (auto k: {1, 2, 3}) {
          auto [w1_c, w2_c] = game_step(scores, pos, i + j + k, npid);
          p1_wins += w1_c;
          p2_wins += w2_c;
        }
    wins_temp[scores[0]][scores[1]][pos[0]][pos[1]][npid] = std::pair{p1_wins, p2_wins};
  }

  return *wins_temp[scores[0]][scores[1]][pos[0]][pos[1]][npid];
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::array<long, 2> p_pos{v[0].start, v[1].start};
  std::array<long, 2> p_score = {0, 0};

  unsigned long long p1_wins = 0, p2_wins = 0;
  for (auto i: {1, 2, 3})
    for (auto j: {1, 2, 3})
      for (auto k: {1, 2, 3}) {
        auto [w1_c, w2_c] = game_step(p_score, p_pos, i + j + k, 0);
        p1_wins += w1_c;
        p2_wins += w2_c;
      }
  return std::max(p1_wins, p2_wins);
}

#include "../exec.hpp"