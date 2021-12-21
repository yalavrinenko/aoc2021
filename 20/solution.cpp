//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <set>
#include <unordered_set>

struct AOC_Output {
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string line;
  explicit AOC_Input(std::string s): line(std::move(s)) {}
};

struct hash_pair{
  size_t operator()(std::pair<long, long> const &p) const {
    return std::hash<long>{}(p.first) ^ std::hash<long>{}(p.second);
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) { return (line.empty()) ? std::optional<AOC_Input>{} : AOC_Input(line); }

auto image_reconstruction(std::string const& enhanced_algo, std::unordered_set<std::pair<long, long>, hash_pair> const& map,
                          std::pair<long, long> &xlim, std::pair<long, long> &ylim, bool outofrange_bit){

  std::unordered_set<std::pair<long, long>, hash_pair> nimage;
  auto &[xmin, xmax] = xlim; xmin -= 1; xmax += 1;
  auto &[ymin, ymax] = ylim; ymin -= 1; ymax += 1;
  for (auto x = xmin; x < xmax; ++x)
    for (auto y = ymin; y < ymax; ++y){
      auto index = 0ul;
      for (auto i = x - 1; i <= x + 1; ++i)
        for (auto j = y - 1; j <= y + 1; ++j){
          if (xmin + 1 <= i && i < xmax - 1 && ymin + 1 <= j && j < ymax - 1)
            index = (index << 1) + map.contains(std::pair{i, j});
          else
            index = (index << 1) + outofrange_bit;
        }
      if (enhanced_algo[index] == '#') nimage.insert(std::pair{x, y});
    }
  return nimage;
}

auto create_image(std::vector<AOC_Input> const& v){
  std::unordered_set<std::pair<long, long>, hash_pair> nimage;
  for (long x = 0; auto const& e : v | std::views::drop(1)){
    for (long y = 0; auto const& c : e.line){
      if (c == '#') nimage.insert(std::pair{x, y});
      ++y;
    }
    ++x;
  }
  std::pair<long, long> xlim{0, v.size() - 1}, ylim{0, v[1].line.size()};
  return std::tuple{nimage, xlim, ylim};
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto const& enhanced_string = v.front().line;

  auto [nimage, xlim, ylim] = create_image(v);

  auto N = 2;

  bool obit = false;

  while (N > 0) {
    nimage = image_reconstruction(enhanced_string, nimage, xlim, ylim, obit);
    obit = !obit;
    --N;
  }

  using namespace std::views;

  return std::ranges::count_if(nimage, [&origin_xlim = xlim, &origin_ylim = ylim](auto const& p){
    return true;
  });
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto const& enhanced_string = v.front().line;

  auto [nimage, xlim, ylim] = create_image(v);

  auto N = 50;

  bool obit = false;

  while (N > 0) {
    nimage = image_reconstruction(enhanced_string, nimage, xlim, ylim, obit);
    obit = !obit;
    --N;
  }

  using namespace std::views;

  return std::ranges::count_if(nimage, [&origin_xlim = xlim, &origin_ylim = ylim](auto const& p){
    return true;
  });
}

#include "../exec.hpp"