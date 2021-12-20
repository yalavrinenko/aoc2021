//
// Created by yalavrinenko on 02.12.2020.
//
#include <utility>

#include "../main.hpp"
#include <stack>
#include <unordered_map>
#include <unordered_set>

struct AOC_Output{
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::string line;
  explicit AOC_Input(std::string s): line{std::move(s)}{
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

auto get_braket_fall(auto const& in){
  std::stack<char> s;
  std::unordered_map<char, char> complement{
      {')', '('},
      {'>', '<'},
      {']', '['},
      {'}', '{'}
  };
  std::unordered_set<char> braket{'(', '{', '<', '['};

  auto it = std::ranges::find_if(in, [&s, &complement, &braket](auto c) {
    if (braket.count(c)){
      s.push(c);
      return false;
    } else {
      if (s.empty() || complement[c] != s.top())
        return true;
      else {
        s.pop(); return false;
      }
    }
  });
  return (it == in.end()) ?  std::pair(std::optional<char>{}, s) : std::pair(std::optional{*it}, s);
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  using namespace std::views;
  auto in_view = v | transform([](auto const& e) { return get_braket_fall(e.line); }) | filter([](auto const& e) { return e.first.has_value(); }) | common;
  std::unordered_map<char, int> scores{
      {')', 3},
      {']', 57},
      {'}', 1197},
      {'>', 25137}
  };
  return std::accumulate(std::begin(in_view), std::end(in_view), 0ul, [&scores](auto sum, auto const& e){
    return sum + scores[*(e.first)];
  });
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  using namespace std::views;
  std::unordered_map<char, int> scores{
      {'(', 1},
      {'[', 2},
      {'{', 3},
      {'<', 4}
  };

  auto in_view = v | transform([](auto const& e) { return get_braket_fall(e.line); })
                 | filter([](auto const& e) { return !e.first.has_value(); })
                 | transform([](auto const& e) { return e.second; })
                 | transform([&scores](auto s) {
                      auto sum = 0ull;
                      for (;!s.empty();s.pop()) sum = sum * 5 + scores[s.top()];
                      return sum;
                   })
                 | common;
  std::vector tour_table(std::begin(in_view), std::end(in_view));

  std::ranges::nth_element(tour_table, tour_table.begin() + (tour_table.size() / 2));

  return tour_table[tour_table.size() / 2];
}

#include "../exec.hpp"