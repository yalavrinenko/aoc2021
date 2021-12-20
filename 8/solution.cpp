//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <cassert>
#include <string>

struct AOC_Output{
  unsigned long value;
  auto operator() () const { return value; }
};

struct AOC_Input{
  std::vector<std::string> train_digits;
  std::vector<std::string> eval_digits;

  explicit AOC_Input(std::string const &s): train_digits(10), eval_digits(4){
    std::istringstream iss(s);
    std::copy_n(std::istream_iterator<std::string>{iss}, 10, std::begin(train_digits));

    auto order_lambda = [](auto &e) { std::ranges::sort(e); };
    std::ranges::for_each(train_digits, order_lambda);

    char tmp; iss >> tmp;
    std::copy_n(std::istream_iterator<std::string>{iss}, 4, std::begin(eval_digits));
    std::ranges::for_each(eval_digits, order_lambda);
  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

auto easy_digit_correction(auto const& train){
  std::unordered_map<int, int> unique_pattern{{2, 1}, {4, 4}, {3, 7}, {7, 8}};

  auto digit_view = train | std::views::filter([&unique_pattern](auto const& e) {
                      return unique_pattern.count(e.size());
                    }) | std::views::transform([&unique_pattern](auto const& e){
                      return std::pair{e, unique_pattern[e.size()]};
                    }) | std::views::common;

  std::unordered_map<std::string, int> digits{digit_view.begin(), digit_view.end()};
  return digits;
}
auto complex_digit_correction(auto& digits, auto const &train){
    using namespace std::views;
    auto inverse_digit_view = digits | transform ([](auto const &kv) { return std::pair{kv.second, kv.first}; }) | common;
    auto inverse_digit = std::unordered_map(inverse_digit_view.begin(), inverse_digit_view.end());

    auto six_digit = train | filter([](auto const &e) { return e.size() == 6; });
    auto center_candidates = six_digit
                  | transform([&eight = inverse_digit[8]](auto const& e) {
                                std::string diff;
                                std::ranges::set_difference(eight, e, std::back_inserter(diff));
                                return diff;
                              });
    auto r_up = *(center_candidates | filter([&one = inverse_digit[1]](auto const& e) { return one.find(e) != std::string::npos; }) | take(1)).begin();

    auto center = *(center_candidates
                  | filter([r_up](auto const& e) { return e != r_up; })
                  | filter([&four = inverse_digit[4]](auto const& e){
                    return four.find(e) != std::string::npos;
                  })
                  | take(1)).begin();
    auto l_down = *(center_candidates | filter([r_up, center](auto const& e) { return e != r_up && e != center; }) | take(1)).begin();

    std::ranges::set_difference(inverse_digit[8], center, std::back_inserter(inverse_digit[0]));
    std::ranges::set_difference(inverse_digit[8], r_up, std::back_inserter(inverse_digit[6]));
    std::ranges::set_difference(inverse_digit[8], l_down, std::back_inserter(inverse_digit[9]));

    std::ranges::set_difference(inverse_digit[9], r_up, std::back_inserter(inverse_digit[5]));

    auto five_digit = train | filter([&inverse_digit](auto const &e) { return e.size() == 5 && inverse_digit[5] != e; });
    inverse_digit[2] = *(five_digit | filter([&l_down](auto const &e) { return e.find(l_down) != std::string::npos; }) | take(1)).begin();
    inverse_digit[3] = *(five_digit | filter([&l_down](auto const &e) { return e.find(l_down) == std::string::npos; }) | take(1)).begin();

    std::ranges::for_each(inverse_digit, [&digits](auto const& kv) {
      digits[kv.second] = kv.first;
    });

    return digits;
}
AOC_Output part_1(std::vector<AOC_Input> const &v){
  auto single_count = [](auto const& entry) {
    auto const &train = entry.train_digits;
    auto const &eval = entry.eval_digits;

    auto digit_map = easy_digit_correction(train);
    auto count = std::ranges::count_if(eval, [&digit_map](auto const &e) { return digit_map.count(e); });
    return count;
  };
  auto overall_count = std::accumulate(v.begin(), v.end(), 0ul, [single_count](auto sum, auto const& entry){
    return sum + single_count(entry);
  });
  return {overall_count};
}

AOC_Output part_2(std::vector<AOC_Input> const &v){
  auto evaluate_func = [](auto const& entry){
    auto const &train = entry.train_digits;
    auto const &eval = entry.eval_digits;

    auto digit_map = easy_digit_correction(train);
    digit_map = complex_digit_correction(digit_map, train);

    auto value = std::accumulate(eval.begin(), eval.end(), 0ul, [&digit_map](auto sum, auto const &e){
      return (sum * 10 + digit_map[e]);
    });
    return value;
  };
  auto sum = std::accumulate(v.begin(), v.end(), 0ul, [evaluate_func](auto sum, auto const& entry) { return sum + evaluate_func(entry); });
  return {sum};
}

#include "../exec.hpp"