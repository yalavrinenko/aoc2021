//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <cassert>
#include <list>
#include <unordered_set>

struct AOC_Output {
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string from, to;
  explicit AOC_Input(std::string s) {
    std::ranges::replace(s, '-', ' ');
    std::istringstream iss{s};
    iss >> from >> to;
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) { return AOC_Input(line); }

using vertex_list = std::unordered_map<std::string, std::list<std::string>>;

auto create_vertex_list(auto const &v) {
  vertex_list L;
  std::ranges::for_each(v, [&L](auto const &e) {
    if (e.to != "start" && e.from != "end")
      L[e.from].emplace_back(e.to);
    if (e.to != "end" && e.from != "start")
      L[e.to].emplace_back(e.from);
  });
  return L;
}

class trivial_visit_monitor {
public:
  bool accessible(std::string const &vertex) const { return !visited_.contains(vertex); }

  void enter(std::string const &vertex) { visited_.insert(vertex); }

  void exit(std::string const &vertex) { visited_.erase(vertex); }

private:
  std::unordered_set<std::string> visited_;
};

auto &distance_map() {
  static std::unordered_map<std::string, long> path_count;
  return path_count;
}

template <typename monitor_strategy>
void perform_dfs(vertex_list const &G, const std::string &vertex, monitor_strategy &monitor) {
  ++distance_map()[vertex];
  if (vertex == "end") {
  } else {
    for (auto &candidates: G.at(vertex)) {
      if (!std::islower(candidates[0])) {
        perform_dfs(G, candidates, monitor);
      } else {
        if (monitor.accessible(candidates)) {
          monitor.enter(candidates);
          perform_dfs(G, candidates, monitor);
          monitor.exit(candidates);
        }
      }
    }
  }
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto G = create_vertex_list(v);

  distance_map().clear();
  trivial_visit_monitor monitor;
  monitor.enter("start");

  perform_dfs(G, "start", monitor);
  return distance_map()["end"];
}

class complex_visit_monitor {
public:
  bool accessible(std::string const &vertex) const { return (!visited_.contains(vertex)) || (visited_.contains(vertex) && !lock_vertex_); }

  void enter(std::string const &vertex) {
    if (visited_.contains(vertex)) lock_vertex_ = vertex;
    else
      visited_.insert(vertex);
  }

  void exit(std::string const &vertex) {
    if (lock_vertex_ && vertex == *lock_vertex_) lock_vertex_.reset();
    else
      visited_.erase(vertex);
  }

private:
  std::optional<std::string> lock_vertex_{};
  std::unordered_set<std::string> visited_;
};

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto G = create_vertex_list(v);
  distance_map().clear();

  complex_visit_monitor monitor;
  monitor.enter("start");

  perform_dfs(G, "start", monitor);
  return distance_map()["end"];
}

#include "../exec.hpp"