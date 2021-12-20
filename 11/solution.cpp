//
// Created by yalavrinenko on 02.12.2020.
//
#include <utility>

#include "../main.hpp"
#include <queue>

struct AOC_Output{
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::string s;
  explicit AOC_Input(std::string s): s{std::move(s)}{
    std::ranges::for_each(this->s, [](auto &e) { e -= 0x30; });
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

auto create_octopus_map(auto const& v) {
  using namespace std::views;
  auto octopus_view = v | transform([](auto const& e) { return e.s; }) | common;
  std::vector<std::string> octopus(octopus_view.begin(), octopus_view.end());
  return octopus;
}

auto evolve(auto &octomap){
  std::queue<std::pair<int, int>> to_flash;
  std::array<std::array<int, 10>, 10> flashed{};

  for (std::weakly_incrementable auto i : std::views::iota(0, 10))
    for (std::weakly_incrementable auto j : std::views::iota(0, 10)) {
      ++octomap[i][j];
      if (octomap[i][j] == 10) to_flash.emplace(i, j);
    }

  auto in_range = [](auto i, auto j){
    return (0 <= i && i < 10) && (0 <= j && j < 10);
  };

  auto count = 0ul;
  while (!to_flash.empty()){
    auto &[ci, cj] = to_flash.front();
    flashed[ci][cj] = true;
    ++count;
    for (std::weakly_incrementable auto i : std::views::iota(-1, 2))
      for (std::weakly_incrementable auto j : std::views::iota(-1, 2)){
        auto nx = ci + i, ny = cj + j;
        if (in_range(nx, ny) && !flashed[nx][ny]){
          octomap[nx][ny] += 1;
          if (octomap[nx][ny] == 10)
            to_flash.emplace(nx, ny);
        }
      }
    octomap[ci][cj] = 0;
    to_flash.pop();
  }

  return count;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto octopus = create_octopus_map(v);
  auto total_flashes = 0;
  for (auto i = 0; i < 100; ++i)
    total_flashes += evolve(octopus);

  return total_flashes;
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto octopus = create_octopus_map(v);
  auto step = 1;
  while (evolve(octopus) != 100)
    ++step;
  return step;
}

#include "../exec.hpp"