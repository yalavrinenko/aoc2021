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
  std::pair<long, long> xlim, ylim;
  explicit AOC_Input(std::string const &s) {
    std::sscanf(s.c_str(), "target area: x=%ld..%ld, y=%ld..%ld", &xlim.first, &xlim.second, &ylim.first, &ylim.second);
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) { return AOC_Input(line); }

long solve_square(long x){
  auto D = std::sqrt(1 + 8 * x);
  auto x1 = std::ceil((-1 + D) / 2.0);
  return x1;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto maxy = v.front().ylim.first;
  return maxy * (maxy + 1) / 2;
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto [xmin, xmax] = v.front().xlim;
  auto [ymin, ymax] = v.front().ylim;

  using namespace std::views;

  auto count = 0ul;


  auto simulate = [xlim = v.front().xlim, ylim = v.front().ylim](auto vx, auto vy){
    auto x = 0, y = 0;
    auto stop_iteration = false;
    while (!stop_iteration){
      x += vx; vx = std::max(vx - 1, 0l);
      y += vy; --vy;
      if ((xlim.first <= x && x <= xlim.second) && (ylim.first <= y && y <= ylim.second))
        return true;

      stop_iteration = x > xlim.second || y < ylim.first;
    }
    return false;
  };

  for (std::weakly_incrementable auto vx : iota(solve_square(xmin), xmax + 1)){
    //auto tmin = vx + 1;
    auto vy_max = -ymin;
    //long vy_min = std::ceil(static_cast<double>(ymax) / static_cast<double>(tmin));
    for (std::weakly_incrementable auto vy : iota(ymin, vy_max)){
      if (simulate(vx, vy)) {
        //fmt::print("({} {}) {}, {}\n", vy_min, vy_max, vx, vy);
        ++count;
      }
    }
  }

  return count;
}

#include "../exec.hpp"