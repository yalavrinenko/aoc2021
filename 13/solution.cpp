//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <unordered_set>

struct AOC_Output{
  unsigned long value;
  AOC_Output(unsigned long value) : value(value) {}
  auto operator() () const { return value; }
};

struct AOC_Input{
  long x{}, y{};
  bool is_command = false;
  enum class FoldDirection{
    horizontal,
    vertical
  };

  FoldDirection axis;

  explicit AOC_Input(std::string const &s){
    std::istringstream iss{s};
    if (s[0] == 'f'){
      std::string tmp;
      iss >> tmp >> tmp;
      char axe; iss >> axe;
      iss.ignore();
      if (axe == 'y') {
        iss >> y;
        this->axis = FoldDirection::horizontal;
      } else {
        iss >> x;
        this->axis = FoldDirection::vertical;
      }
      is_command = true;
    } else {
      iss >> x;
      iss.ignore();
      iss >> y;
    }
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return (line.empty()) ? std::optional<AOC_Input>{} : AOC_Input(line);
}

struct pairhash {
public:
  template <typename T, typename U>
  std::size_t operator()(const std::pair<T, U> &x) const  {
    return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
  }
};

auto& fold_paper(auto &paper, AOC_Input const& fold_command){
  using namespace std::views;
  if (fold_command.axis == AOC_Input::FoldDirection::horizontal){
    auto to_fold = paper | filter([y = fold_command.y](auto const& e) { return e.second > y; });
    std::ranges::for_each(to_fold, [&paper, &y = fold_command.y](auto &e){
      e = std::pair{e.first, y - (e.second - y)};
    });
  } else {
    auto to_fold = paper | filter([x = fold_command.x](auto const &e) { return e.first > x; });
    std::ranges::for_each(to_fold, [&paper, &x = fold_command.x](auto &e){
      e = std::pair{x - (e.first - x), e.second};
    });
  }

  return paper;
}

AOC_Output part_1(std::vector<AOC_Input> const &v){
  using namespace std::views;
  auto paper_view = v | filter([](auto const& e) { return !e.is_command; }) | transform([](auto const& e) { return std::pair{e.x, e.y}; }) | common;
  std::vector<std::pair<long, long>> paper(std::begin(paper_view), std::end(paper_view));

  auto command = (v | filter([](auto const& e) { return e.is_command; }) | take(1)).front();

  [[maybe_unused]] auto print = [&paper]() {
    fmt::print("Size = {}:", paper.size());
    for (auto const& e: paper)
      fmt::print("({}, {})\t", e.first, e.second);
    fmt::print("\n");
  };

  paper = fold_paper(paper, command);

  std::unordered_set<std::pair<long, long>, pairhash> paper_unique(paper.begin(), paper.end());

  return paper_unique.size();
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  using namespace std::views;
  auto paper_view = v | filter([](auto const& e) { return !e.is_command; }) | transform([](auto const& e) { return std::pair{e.x, e.y}; }) | common;
  std::vector<std::pair<long, long>> paper(std::begin(paper_view), std::end(paper_view));

  auto command = v | filter([](auto const& e) { return e.is_command; });

  for (auto const& c : command)
    paper = fold_paper(paper, c);

  std::unordered_set<std::pair<long, long>, pairhash> paper_unique(paper.begin(), paper.end());

  auto max_x = std::ranges::max(paper_unique, {}, &std::pair<long, long>::first);
  auto max_y = std::ranges::max(paper_unique, {}, &std::pair<long, long>::second);

  std::vector<std::vector<char>> map(max_y.second + 1, std::vector<char>(max_x.first + 1, ' '));
  std::ranges::for_each(paper_unique, [&map](auto const& e) {
    map[e.second][e.first] = '@';
  });

  std::ranges::for_each(map, [](auto const& line) {
    std::ranges::copy(line, std::ostream_iterator<char>{std::cout, ""});
    std::cout << std::endl;
  });

  return paper_unique.size();
}

#include "../exec.hpp"