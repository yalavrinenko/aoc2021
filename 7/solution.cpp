//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <cmath>

struct AOC_Output{
  long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::vector<int> values;
  explicit AOC_Input(std::string s){
    std::replace(std::begin(s), std::end(s), ',', ' ' );
    std::istringstream iss{s};
    std::copy(std::istream_iterator<int>{iss}, std::istream_iterator<int>{}, std::back_inserter(values));
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

auto fuel_consumption(std::vector<AOC_Input> const& v, auto fuel_evalu_func){
  auto const& values = v.front().values;

  auto [min, max] = std::ranges::minmax(values);

  auto eval_fuel = [&values, fuel_evalu_func](auto median) {
    return std::accumulate(std::begin(values), std::end(values), 0, [median, fuel_evalu_func](auto sum, auto e){
      return sum + fuel_evalu_func(median, e);
    });
  };

  auto min_fuel = std::ranges::min(std::views::iota(min, max), {}, eval_fuel);
  return eval_fuel(min_fuel);
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  return {fuel_consumption(v, [](auto opt_pos, auto e) { return std::abs(opt_pos - e); })};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto fuel_eval_f = [](auto opt_pos, auto e) {
    return  (1 + std::abs(e - opt_pos)) * std::abs(e - opt_pos) / 2;
  };
  return {fuel_consumption(v, fuel_eval_f)};
}

#include "../exec.hpp"