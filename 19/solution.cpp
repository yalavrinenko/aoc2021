//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <queue>
#include <set>
#include <fmt/ostream.h>

struct AOC_Output {
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator()() const { return value; }
};

struct point_3d {
  long x, y, z;
  point_3d operator-(point_3d const &rhs) const { return point_3d{x - rhs.x, y - rhs.y, z - rhs.z}; }
  point_3d operator+(point_3d const &rhs) const { return point_3d{x + rhs.x, y + rhs.y, z + rhs.z}; }

  bool operator==(point_3d const& other) const {
    return x == other.x && y == other.y && z == other.z;
  }
};

std::ostream& operator<< (std::ostream &out, point_3d const& p){
  return (out << p.x << ", " << p.y << ", " << p.z);
}

bool operator<(point_3d const &lhs, point_3d const &rhs) {
  if (lhs.x == rhs.x) { return (lhs.y == rhs.y) ? lhs.z < rhs.z : lhs.y < rhs.y; }
  return lhs.x < rhs.x;
};



class MathUtil {
public:
  static long distance_sqr(point_3d const &a, point_3d const &b) {
    auto sqr = [](auto x) { return x * x; };
    return sqr(a.x - b.x) + sqr(a.y - b.y) + sqr(a.z - b.z);
  }
};

struct scanner {
  std::optional<point_3d> r;
  std::vector<point_3d> beacons;
};

struct AOC_Input {
  scanner s;
  explicit AOC_Input(scanner s) : s{std::move(s)} {}
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  static scanner s;
  if (line.empty()) {
    auto cmp = [](point_3d const &lhs, point_3d const &rhs) {
      if (lhs.x == rhs.x) { return (lhs.y == rhs.y) ? lhs.z < rhs.z : lhs.y < rhs.y; }
      return lhs.x < rhs.x;
    };
    std::ranges::sort(s.beacons, cmp);
    return AOC_Input(s);
  }

  if (line.find("---") == std::string::npos) {
    auto cline = line;
    std::ranges::replace(cline, ',', ' ');
    std::istringstream iss{cline};
    long x, y, z;
    iss >> x >> y >> z;
    s.beacons.emplace_back(point_3d{x, y, z});
  } else {
    s.beacons.clear();
  }
  return {};
}
struct distance {
  size_t i, j;
  long dsqr;
};
class Utils {
public:
  static auto beacons_distance_map(scanner const &s) {
    std::vector<distance> map;
    for (auto i = 0ul; i < s.beacons.size() - 1; ++i)
      for (auto j = i + 1ul; j < s.beacons.size(); ++j) { map.emplace_back(distance{i, j, MathUtil::distance_sqr(s.beacons[i], s.beacons[j])}); }
    std::ranges::sort(map, {}, &distance::dsqr);
    return map;
  }

  static auto beacons_intersection(scanner const &lhs, scanner const &rhs) {
    auto lhs_map = beacons_distance_map(lhs);
    auto rsh_map = beacons_distance_map(rhs);

    std::vector<distance> intersection_lhs, intersection_rhs;
    std::ranges::set_intersection(lhs_map, rsh_map, std::back_inserter(intersection_lhs), {}, &distance::dsqr, &distance::dsqr);
    std::ranges::set_intersection(rsh_map, lhs_map, std::back_inserter(intersection_rhs), {}, &distance::dsqr, &distance::dsqr);

    std::set<size_t> lhs_index, rhs_index;
    for (auto i = 0ul; i < intersection_lhs.size(); ++i) {
      lhs_index.insert(intersection_lhs[i].i);
      lhs_index.insert(intersection_lhs[i].j);

      rhs_index.insert(intersection_rhs[i].i);
      rhs_index.insert(intersection_rhs[i].j);
    }

    std::vector<point_3d> lhs_beacons, rhs_beacons;
    std::ranges::transform(lhs_index, std::back_inserter(lhs_beacons), [&lhs](auto i) { return lhs.beacons[i]; });
    std::ranges::transform(rhs_index, std::back_inserter(rhs_beacons), [&rhs](auto i) { return rhs.beacons[i]; });
    return std::pair{lhs_beacons, rhs_beacons};
  }

  static auto offset(std::vector<point_3d> const& v) {
    std::vector<point_3d> offsets;
    offsets.reserve(v.size() - 1);
    for (auto i = 0ul; i < v.size() - 1; ++i) offsets.emplace_back(v[i] - v[i + 1]);
    return offsets;
  }

  static auto match_points(scanner const &ref, scanner &s, std::vector<point_3d> ref_int, std::vector<point_3d> s_int);

private:
  static auto transformation() {
    using func_t = std::function<point_3d(point_3d const &)>;

    std::vector<func_t> transform{
        [](auto const& p) { return point_3d{p.x, p.y, p.z}; },
        [](auto const& p) { return point_3d{p.x, -p.z, p.y}; },
        [](auto const& p) { return point_3d{p.x, -p.y, -p.z}; },
        [](auto const& p) { return point_3d{p.x, p.z, -p.y}; },

        [](auto const& p) { return point_3d{-p.x, -p.y, p.z}; },
        [](auto const& p) { return point_3d{-p.x, p.z, p.y}; },
        [](auto const& p) { return point_3d{-p.x, p.y, -p.z}; },
        [](auto const& p) { return point_3d{-p.x, -p.z, -p.y}; },

        [](auto const& p) { return point_3d{p.y, p.z, p.x}; },
        [](auto const& p) { return point_3d{p.y, -p.x, p.z}; },
        [](auto const& p) { return point_3d{p.y, -p.z, -p.x}; },
        [](auto const& p) { return point_3d{p.y, p.x, -p.z}; },

        [](auto const& p) { return point_3d{-p.y, -p.z, p.x}; },
        [](auto const& p) { return point_3d{-p.y, p.x, p.z}; },
        [](auto const& p) { return point_3d{-p.y, p.z, -p.x}; },
        [](auto const& p) { return point_3d{-p.y, -p.x, -p.z}; },

        [](auto const& p) { return point_3d{p.z, p.x, p.y}; },
        [](auto const& p) { return point_3d{p.z, -p.y, p.x}; },
        [](auto const& p) { return point_3d{p.z, -p.x, -p.y}; },
        [](auto const& p) { return point_3d{p.z, p.y, -p.x}; },

        [](auto const& p) { return point_3d{-p.z, -p.x, p.y}; },
        [](auto const& p) { return point_3d{-p.z, p.y, p.x}; },
        [](auto const& p) { return point_3d{-p.z, p.x, -p.y}; },
        [](auto const& p) { return point_3d{-p.z, -p.y, -p.x}; },
    };
    return transform;
  }
};
auto Utils::match_points(const scanner &ref, scanner &s, std::vector<point_3d> ref_int, std::vector<point_3d> s_int) {
  auto tfuncs = Utils::transformation();

//  auto cmp = [](point_3d const &lhs, point_3d const &rhs) {
//    if (lhs.x == rhs.x) { return (lhs.y == rhs.y) ? lhs.z < rhs.z : lhs.y < rhs.y; }
//    return lhs.x < rhs.x;
//  };
//  std::ranges::sort(ref_int, cmp);


  auto ref_offset = Utils::offset(ref_int);

  for (auto const& tf : tfuncs){
    std::vector<point_3d> after(s_int.size());
    std::ranges::transform(s_int, std::begin(after), tf);

//    std::ranges::sort(after, cmp);
    auto after_offset = Utils::offset(after);

    if (after_offset == ref_offset){

      for (auto i = 0ul; i < after.size(); ++i)
        fmt::print("({}) == ({})\n", ref_int[i], after[i]);

      auto rpoint = ref.r.value() + ref_int[0];
      s.r = rpoint - after[0];
      std::ranges::transform(s.beacons, std::begin(s.beacons), [&s, tf](auto p){
        p = tf(p);
        p = s.r.value() + p;
        return p;
      });
//      return s;
    }
  }

  throw std::logic_error("Wft?!");
}


AOC_Output part_1(std::vector<AOC_Input> const &vv) {
  auto v = vv;
  std::set<size_t> known_position;
  std::queue<size_t> next_comp;

  known_position.insert(0);
  next_comp.push(0);
  v[0].s.r = point_3d{0, 0, 0};

  while (!next_comp.empty()) {
    auto i = next_comp.front();
    for (auto j = 0ul; j < v.size(); ++j) {
      if (!known_position.contains(j)) {
        auto [lbeacons, rbeacons] = Utils::beacons_intersection(v[i].s, v[j].s);
        if (lbeacons.size() >= 12) {
          Utils::match_points(v[i].s, v[j].s, lbeacons, rbeacons);

          //fmt::print("Scanner {} position is ({},{},{})\n", j, v[j].s.r->x, v[j].s.r->y, v[j].s.r->z);

          next_comp.push(j);
          known_position.insert(j);
        }
      }
    }

    next_comp.pop();
  }

  std::set<point_3d> all_beacons;
  for (auto const& e : v){
    all_beacons.insert(e.s.beacons.begin(), e.s.beacons.end());
  }

  //  std::ranges::for_each(all_beacons, [](auto const& e){
  //    fmt::print("{},{},{}\n", e.x, e.y, e.z);
  //  });

  return all_beacons.size();
}


AOC_Output part_2(std::vector<AOC_Input> const &v) { return 0; }

#include "../exec.hpp"