//
// Created by yalavrinenko on 06.12.2021.
//
#include "../main.hpp"
#include <fmt/core.h>
#include <array>

struct AOC_Output {
  unsigned long long value;

  auto operator()() const { return value; }
};

struct AOC_Input {
  std::vector<long> fishes;

  explicit AOC_Input(std::string s) {
    std::replace(s.begin(), s.end(), ',', ' ');
    std::istringstream ss(s);
    std::copy(std::istream_iterator<long>{ss}, std::istream_iterator<long>{}, std::back_inserter(fishes));
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

std::array<long long, 256> fish_memory{};

unsigned long long fish_count(long current, long epoch) {
  auto count = 1ull;
  auto const period = 6;

  //fmt::print("start: {} epoch {}\n", current, epoch);

  while (epoch > 0) {
    if (current == 0) {
      if (fish_memory[epoch - 1] == 0)
        fish_memory[epoch - 1] = fish_count(8, epoch - 1);
      count += fish_memory[epoch - 1];
      current = period;
    } else
      --current;
    --epoch;
  }

  return count;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto fishes = std::accumulate(v.front().fishes.begin(), v.front().fishes.end(), 0ull, [](auto sum, auto fish) {
    return sum + fish_count(fish, 80);
  });

  return {fishes};
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto fishes = std::accumulate(v.front().fishes.begin(), v.front().fishes.end(), 0ull, [](auto sum, auto fish) {
    return sum + fish_count(fish, 256);
  });
  return {fishes};
}

#include "../exec.hpp"