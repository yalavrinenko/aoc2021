//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <cassert>
#include <variant>

struct AOC_Output {
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string line;
  explicit AOC_Input(std::string s) : line(std::move(s)) {}
};

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) { return AOC_Input(line); }
template<typename... T>
class visitors : public T... {
public:
  using T::operator()...;
};

template<typename... T>
visitors(T...) -> visitors<T...>;

class machine {
public:
  using reg = std::string;
  using input = std::variant<long, reg>;
  using instruction_code = std::string;
  using instruction = std::function<void(input const &, input const &)>;

  struct alu_entry {
    instruction op;
    std::optional<input> a, b;
    alu_entry(std::string const &s, auto const &alu_instruction);

    void run() { op(*a, ((b) ? *b : input{0l})); }
  };

  explicit machine(std::vector<AOC_Input> const &in);

  void reset() {
    alu_registers_.clear();
    alu_registers_["x"] = alu_registers_["y"] = alu_registers_["z"] = alu_registers_["w"] = 0;
  }

  void set_input(std::string const &s) { input_stream_ = std::istringstream{s}; }

  void execute() {
    for (auto &e: entry_list_) e.run();
  }

  [[nodiscard]] auto const &alu_state() const { return alu_registers_; }

private:
  auto alu_instructions();

  std::vector<alu_entry> entry_list_;
  std::unordered_map<reg, long> alu_registers_;
  std::istringstream input_stream_;
};

machine::alu_entry::alu_entry(const std::string &s, auto const &alu_instruction) {
  std::istringstream iss{s};
  instruction_code code;
  iss >> code;
  op = alu_instruction.at(code);
  reg r;
  iss >> r;
  a = r;
  if (code != "inp") {
    reg r_b;
    iss >> r_b;
    if (std::isdigit(r_b[0]) || r_b[0] == '-') {
      b = std::stol(r_b);
    } else
      b = r_b;
  }
}

auto machine::alu_instructions() {
  std::unordered_map<instruction_code, instruction> alu_instr;
  alu_instr["inp"] = [this](input const &a, input const &) {
    char c;
    input_stream_ >> c;
    alu_registers_[std::get<reg>(a)] = c - '0';
  };

  auto math_op = [this](input const &a, input const &b, auto op) {
    auto on_val = [this, &a, op](long bv) { alu_registers_[std::get<reg>(a)] = op(alu_registers_[std::get<reg>(a)], bv); };
    auto on_reg = [this, &a, op](reg const &r) { alu_registers_[std::get<reg>(a)] = op(alu_registers_[std::get<reg>(a)], alu_registers_[r]); };
    std::visit(visitors{on_val, on_reg}, b);
  };

  alu_instr["add"] = [this, math_op](input const &a, input const &b) { math_op(a, b, std::plus<long>{}); };

  alu_instr["mul"] = [this, math_op](input const &a, input const &b) { math_op(a, b, std::multiplies<long>{}); };

  alu_instr["div"] = [this, math_op](input const &a, input const &b) { math_op(a, b, std::divides<long>{}); };

  alu_instr["mod"] = [this, math_op](input const &a, input const &b) { math_op(a, b, std::modulus<long>{}); };

  alu_instr["eql"] = [this, math_op](input const &a, input const &b) { math_op(a, b, std::equal_to<long>{}); };

  return alu_instr;
}

machine::machine(const std::vector<AOC_Input> &in) {
  auto eview = in | std::views::transform([this](auto const &e) { return alu_entry(e.line, alu_instructions()); }) | std::views::common;
  entry_list_ = std::vector<alu_entry>{std::begin(eview), std::end(eview)};

  reset();
}

auto smart_eval(std::string const &digits) {
  long x_add[] = {11, 11, 14, 11, -8, -5, 11, -13, 12, -1, 14, -5, -4, -8};
  long z_div[] = {1, 1, 1, 1, 26, 26, 1, 26, 1, 26, 1, 26, 26, 26};
  long w_add[] = {1, 11, 1, 11, 2, 9, 7, 11, 6, 15, 7, 1, 8, 6};

  assert(sizeof(z_div) == sizeof(z_div));
  long x = 0, z = 0;
  for (auto i = 0; i < 14; ++i) {
    x = (z % 26 + x_add[i] == digits[i]) ? 0 : 1;
    z = (z / z_div[i]) * (25 * x + 1) + (digits[i] + w_add[i]) * x;
  }
  return z;
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  machine m(v);

  std::string num = "92969593497992";

  m.reset();
  m.set_input(num);
  m.execute();
  fmt::print("{} ==> {}\n", num, m.alu_state().at("z"));

  return std::stol(num);
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  machine m(v);

  std::string num = "81514171161381";

  m.reset();
  m.set_input(num);
  m.execute();
  fmt::print("{} ==> {}\n", num, m.alu_state().at("z"));

  return std::stol(num);
}

#include "../exec.hpp"