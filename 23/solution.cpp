//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"

struct AOC_Output{
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator() () const { return value; }
};

struct AOC_Input{

  explicit AOC_Input(std::string const &s){

  }
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) {
  return AOC_Input(line);
}

AOC_Output part_1(std::vector<AOC_Input> const &v){

}

AOC_Output part_2(std::vector<AOC_Input> const &v){

}

#include "../exec.hpp"