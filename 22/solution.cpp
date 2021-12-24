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
  bool is_on;
  std::pair<long, long> xlim;
  std::pair<long, long> ylim;
  std::pair<long, long> zlim;

  explicit AOC_Input(std::string const &s) {
    char mode[4];
    std::sscanf(s.c_str(), "%s x=%ld..%ld,y=%ld..%ld,z=%ld..%ld", mode, &xlim.first, &xlim.second, &ylim.first, &ylim.second, &zlim.first,
                &zlim.second);
    is_on = std::string_view(mode) == "on";
  }
};

struct Box {
  struct Line {
    long min, max;
  };

  Line x, y, z;

  explicit Box(AOC_Input const &in)
      : x{in.xlim.first, in.xlim.second + 1}, y{in.ylim.first, in.ylim.second + 1}, z{in.zlim.first, in.zlim.second + 1} {}

  Box(Line x, Line y, Line z) : x{x}, y{y}, z{z} {}

  [[nodiscard]] auto volume() const { return (x.max - x.min) * (y.max - y.min) * (z.max - z.min); }
};

bool line_intersected(auto const& a, auto const& b){
  auto [left, right] = std::ranges::minmax(a, b, {}, &Box::Line::min);
  return (right.min < left.max);
}

std::optional<std::array<long, 4>> line_instersection(auto const &a, auto const &b) {
  auto [left, right] = std::ranges::minmax(a, b, {}, &Box::Line::min);
  if (right.min < left.max) {
    return std::array<long, 4>{left.min, right.min, std::min(right.max, left.max), std::max(right.max, left.max)};
  } else
    return {};
};

bool box_intersected(Box const &a, Box const &b) {
  auto xintersection = line_intersected(a.x, b.x);
  auto yintersection = line_intersected(a.y, b.y);
  auto zintersection = line_intersected(a.z, b.z);
  return xintersection && yintersection && zintersection;
}

auto box_intersection(Box const &a, Box const &b) {
  auto xintersection = line_instersection(a.x, b.x);
  auto yintersection = line_instersection(a.y, b.y);
  auto zintersection = line_instersection(a.z, b.z);
  if (xintersection && yintersection && zintersection) {
    std::vector<Box> la, lb, lint;
    for (auto i = 0; i < 3; ++i)
      for (auto j = 0; j < 3; ++j)
        for (auto k = 0; k < 3; ++k) {
          Box nb{Box::Line{xintersection.value()[i], xintersection.value()[i + 1]}, Box::Line{yintersection.value()[j], yintersection.value()[j + 1]},
                 Box::Line{zintersection.value()[k], zintersection.value()[k + 1]}};
          if (nb.volume() != 0) {
            if (box_intersected(a, nb) && box_intersected(nb, b)) lint.push_back(nb);
            else if (box_intersected(a, nb))
              la.push_back(nb);
            else if (box_intersected(nb, b))
              lb.push_back(nb);
          }
        }
    return std::tuple{la, lint, lb};
  } else {
    return std::tuple{std::vector{a}, std::vector<Box>{}, std::vector{b}};
  }
}

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) { return AOC_Input(line); }

void relax_box_list_on(std::vector<Box> &list) {
  bool need_relaxation = true;
  while (need_relaxation) {
    need_relaxation = false;
    for (auto it = list.begin(); it != std::prev(list.end()) && !need_relaxation; ++it)
      for (auto itj = std::next(it); itj != list.end() && !need_relaxation; ++itj) {
        if (box_intersected(*it, *itj)) {
          need_relaxation = true;
          auto [la, lint, lb] = box_intersection(*it, *itj);
          std::swap(*it, list.back());
          list.pop_back();
          list.insert(list.end(), la.begin(), la.end());
        }
      }
  }
}

void disable_cubes(std::vector<Box> &list, Box const &d) {
  bool disable_is_end = false;
  while (!disable_is_end) {
    disable_is_end = true;
    for (auto it = list.begin(); it != list.end() && disable_is_end; ++it) {
      if (box_intersected(d, *it)) {
        disable_is_end = false;
        auto [A, Int, B] = box_intersection(*it, d);
        std::swap(*it, list.back());
        list.pop_back();
        list.insert(list.end(), A.begin(), A.end());
      }
    }
  }
}

auto space_volume(auto const &l) {
  auto vol = 0ul;
  for (auto const &c: l) { vol += c.volume(); }
  return vol;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  std::vector<Box> active_boxes{Box(v.front())};

  auto inrange = [](auto axes) { return (-50 <= axes.first && axes.second <= 50); };

  for (auto const &b: v | std::views::drop(1)) {
//    fmt::print("V = {}\n", space_volume(active_boxes));
    if (inrange(b.xlim) && inrange(b.ylim) && inrange(b.zlim)) {
      if (b.is_on) {
        active_boxes.emplace_back(b);
        relax_box_list_on(active_boxes);
      } else {
        disable_cubes(active_boxes, Box(b));
      }
    }
  }
  return space_volume(active_boxes);
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  std::vector<Box> active_boxes{Box(v.front())};

  for (auto const &b: v | std::views::drop(1)) {
//    fmt::print("V = {}\n", space_volume(active_boxes));
    if (b.is_on) {
      active_boxes.emplace_back(b);
      relax_box_list_on(active_boxes);
    } else {
      disable_cubes(active_boxes, Box(b));
    }
  }
  return space_volume(active_boxes);
}

#include "../exec.hpp"