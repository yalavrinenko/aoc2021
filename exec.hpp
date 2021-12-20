//
// Created by yalavrinenko on 02.12.2020.
//

#ifndef AOC_2020_EXEC_HPP
#define AOC_2020_EXEC_HPP

#include <chrono>

std::vector<AOC_Input> AOC_ParceInput(std::string const &path) {
  std::ifstream in(path);
  std::string line;
  std::vector<AOC_Input> v;
  while (std::getline(in, line)){
    auto input_data = AOC_Reader::create_from_string(line);
    if (input_data)
      v.emplace_back(input_data.value());
  }
  return v;
}

template<typename func_t, typename ... args_t>
void execute_with_timer(std::string_view message, func_t func, args_t ... args){
  auto stime = std::chrono::high_resolution_clock::now();
  auto result = func(std::forward<args_t...>(args...));
  auto ftime = std::chrono::high_resolution_clock::now();
  fmt::print(fmt::emphasis::bold | fg(fmt::color::green), "{}: {}\n", message, result());
  fmt::print(fmt::emphasis::bold | fg(fmt::color::red),"Working time {} μs\n\n",
             std::chrono::duration_cast<std::chrono::microseconds>(ftime - stime).count());
}

int main(int argc, char** argv){
  execute_with_timer("Part 1", part_1, AOC_ParceInput(DAY_PATH));
  execute_with_timer("Part 2", part_2, AOC_ParceInput(DAY_PATH));
  return 0;
}


#endif//AOC_2020_EXEC_HPP
