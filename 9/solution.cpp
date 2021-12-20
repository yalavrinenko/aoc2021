//
// Created by yalavrinenko on 02.12.2020.
//
#include <utility>
#include <set>
#include <list>
#include <queue>

#include "../main.hpp"

struct AOC_Output{
  long value;
  AOC_Output(long v) : value{v} {}
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

auto get_lower_points(auto const& v){
  std::list<std::pair<int, int>> minimal_height;

  auto check_neight = [&v](auto x, auto y) {
    for (auto [i, j] : {std::pair{-1, 0}, {1, 0}, {0, -1}, {0, 1}}){
      auto x_ind = x + i, y_ind = y + j;
      if (x_ind < 0 || y_ind < 0) continue;
      if (x_ind >= static_cast<int>(v.size()) || y_ind >= static_cast<int>(v.front().line.size())) continue;
      if (v[x_ind].line[y_ind] <= v[x].line[y])
        return false;
    }
    return true;
  };

  for (std::weakly_incrementable auto i : std::views::iota(0, static_cast<int>(v.size())))
    for (std::weakly_incrementable auto j : std::views::iota(0, static_cast<int>(v.front().line.size()))){
      if (check_neight(i, j))
        minimal_height.emplace_back(i, j);
    }
  return minimal_height;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto minimal_height = get_lower_points(v);

  return std::accumulate(std::begin(minimal_height), std::end(minimal_height), 0,
                         [&v](auto sum, auto const &e) { return sum + (v[e.first].line[e.second] - 0x30 + 1); });
}

auto find_basin(auto const& v, auto x, auto y) {
  auto getter = [&v](auto i, auto j) -> std::optional<int>{
    if (i < 0 || j < 0) return {};
    if (i >= static_cast<int>(v.size()) || j >= static_cast<int>(v.front().line.size())) return {};
    return std::optional{v[i].line[j]};
  };

  std::set visited{std::pair{x, y}};
  std::queue<std::pair<int, int>> q;
  q.emplace(x, y);
  auto in_basin = 0ul;
  while (!q.empty()){
    ++in_basin;
    auto &current = q.front();

    for (auto [i, j] : {std::pair{current.first, current.second + 1}, {current.first, current.second - 1},
                       {current.first + 1, current.second}, {current.first - 1, current.second}}){
      if (getter(i, j) && visited.count({i, j}) == 0 && getter(i, j) != '9') {
        q.emplace(i, j);
        visited.emplace(i, j);
      }
    }

    q.pop();
  }

  return in_basin;
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto mins_pos = get_lower_points(v);

  using namespace std::views;
  auto depth_view = mins_pos | transform([&v](auto const &kv) { return find_basin(v, kv.first, kv.second); }) | common;
  std::vector basin_sizes(std::begin(depth_view), std::end(depth_view));
  std::ranges::sort(basin_sizes);

  auto largest = basin_sizes | reverse | take(3) | common;
//  for (auto e : largest)
//    fmt::print("{} ", e);
  return std::accumulate(std::begin(largest), std::end(largest), 1, [](auto sum, auto e) { return sum * e; });
}

#include "../exec.hpp"