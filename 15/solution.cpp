//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <queue>

struct AOC_Output{
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::string line;
  explicit AOC_Input(std::string const &s){
    std::ranges::transform(s, std::back_inserter(line), [](auto c) { return c - 0x30; });
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

auto make_map(auto const& v){
  std::vector<std::string> G(v.size());
  std::ranges::transform(v, std::begin(G), [](auto const& e) { return e.line; });
  return G;
}

auto shortest_path(auto const& G){
  struct vertex{
    long i, j;
    long distance;

    bool operator< (vertex const& rhs) const {
      return distance > rhs.distance;
    }
  };

  std::priority_queue<vertex> q;
  std::vector<std::vector<bool>> visited(G.size(), std::vector<bool>(G.front().size()));
  std::vector<std::vector<long>> d(G.size(), std::vector<long>(G.front().size(), std::numeric_limits<long>::max()));

  //using vertex_short = std::pair<long, long>;
  //std::vector<std::vector<vertex_short>> d(G.size(), std::vector<vertex_short>(G.front().size()));

  q.emplace(0, 0, 0);
  d[0][0] = 0;

  while (!q.empty()){
    auto &[i, j, dst] = q.top();
    visited[i][j] = true;

    auto valid_vertex = [&G](unsigned long x, unsigned long y){
      return (0 <= x && x < G.size()) && (0 <= y && y < G.front().size());
    };

    for (auto const [dx, dy] : {std::pair{0, -1}, {0, 1}, {-1, 0}, {1, 0}}){
      auto x = i + dx, y = j + dy;
      if (valid_vertex(x, y) && !visited[x][y] && d[x][y] > G[x][y] + d[i][j]){
        d[x][y] = G[x][y] + d[i][j];
        q.emplace(x, y, G[x][y] + d[i][j]);
      }
    }
    q.pop();
  }

  return d.back().back();
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto G = make_map(v);

  return shortest_path(G);
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto G = make_map(v);
  auto origin = G;
  for (auto i = 0; i < 4; ++i){
    for (auto &line : origin) {
      std::ranges::transform(line, line.begin(), [](auto const& e) { return e == 9 ?  1 : e + 1; });
      G.emplace_back(line);
    }
  }

  auto extend_line = [](auto line) {
    auto origin = line;
    for (auto i = 0; i < 4; ++i) {
      std::ranges::transform(origin, std::begin(origin), [](auto c) { return c == 9 ? 1 : c + 1; });
      line.append(origin);
    }
    return line;
  };

  std::ranges::transform(G, G.begin(), [extend_line](auto const &e){
    return extend_line(e);
  });

  return shortest_path(G);
}

#include "../exec.hpp"