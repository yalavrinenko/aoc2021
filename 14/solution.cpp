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
  std::string main_line;
  std::string init;
  char react;
  explicit AOC_Input(std::string const &s) {
    bool is_reaction = s.find("->") != std::string::npos;
    if (!is_reaction) {
      main_line = s;
    } else {
      init = s.substr(0, 2);
      react = s[s.length() - 1];
    }
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return (!line.empty()) ? AOC_Input(line) : std::optional<AOC_Input>{};
}

auto perform_reaction_step(auto const &components_map, auto &symbol_count, auto const &reaction_map) {
  std::unordered_map<std::string, unsigned long> next_chain;

  for (auto const &[k, v]: components_map) {
    auto first_pair = std::string{k[0], reaction_map.at(k)};
    auto second_pair = std::string{reaction_map.at(k), k[1]};
    next_chain[first_pair] += v;
    next_chain[second_pair] += v;
    symbol_count[reaction_map.at(k)] += v;
  }
  return next_chain;
}

auto create_components_map(std::string const &in_line) {
  std::unordered_map<std::string, unsigned long> components;

  std::unordered_map<char, unsigned long> symbol_counter;

  for (std::weakly_incrementable auto i: std::views::iota(0ul, in_line.size() - 1)) { components[in_line.substr(i, 2)] += 1; }
  for (auto const &c: in_line) symbol_counter[c] += 1;

  return std::pair{components, symbol_counter};
}

auto poly_process(auto const& v, size_t steps){
  auto const &init_str = v.front().main_line;
  using namespace std::views;
  auto reaction_view = v | drop(1) | transform([](auto const &e) { return std::pair<std::string const, char>{e.init, e.react}; }) | common;

  std::unordered_map<std::string, char> reaction_map(std::begin(reaction_view), std::end(reaction_view));

  auto [components_map, symbol_count] = create_components_map(init_str);

  //  fmt::print("Step = {}: ", -1);
  //  std::ranges::for_each(symbol_count, [](auto const &e) { fmt::print("{} = {},", e.first, e.second); });
  //  fmt::print("\n");

  for (auto i = 0ul; i < steps; ++i) {
    components_map = perform_reaction_step(components_map, symbol_count, reaction_map);
    //    fmt::print("Step = {}: ", i);
    //    std::ranges::for_each(symbol_count, [](auto const &e) { fmt::print("{} = {},", e.first, e.second); });
    //    fmt::print("\n");
  }

  auto [min, max] = std::ranges::minmax(symbol_count | values);
  return max - min;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  return poly_process(v, 10);
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  return poly_process(v, 40);
}

#include "../exec.hpp"