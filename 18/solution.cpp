//
// Created by yalavrinenko on 02.12.2020.
//
#include <utility>
#include <variant>

#include "../main.hpp"

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

template<typename... lambda_t>
struct visitors : public lambda_t... {
  using lambda_t::operator()...;
};
template<typename... lambda_t>
visitors(lambda_t...) -> visitors<lambda_t...>;

class snail_number {
public:
  snail_number(std::istringstream &iss, snail_number *parent);

  snail_number(long v, snail_number *parent) : value_{v}, parent_{parent} {}

  snail_number(snail_number* lhs, snail_number* rhs): left_{lhs}, right_{rhs} {
    left_->parent_ = this;
    right_->parent_ = this;
  }

  void dump() const;

  bool explosion_chain(){
    auto action = false;
    while (auto candidate = find_explode_candidate()) {
      perform_explosion(candidate);
//      fmt::print("---");
//      dump();
//      fmt::print("\n");
      action = true;
    }
    return action;
  }
  bool split_chain(){
    auto action = false;
    while (auto sc = find_split_candidate()){
      sc->left_ = new snail_number(sc->value_.value() / 2, sc);
      sc->right_ = new snail_number(static_cast<double>(sc->value_.value()) / 2.0 + 0.5, sc);
      sc->value_ = {};
      action = true;
    }
    return action;
  }

  void reduce() {
    auto flag = explosion_chain();
    flag = flag || split_chain();
    while (flag){
      flag = explosion_chain();
      flag = flag || split_chain();
    }
  }

  ~snail_number() {
    delete left_;
    delete right_;
  }

private:
  snail_number* find_explode_candidate(long depth = 1);
  [[nodiscard]] snail_number* find_split_candidate();

  void perform_explosion(snail_number *target_node);

  void add_left(long v);
  void add_right(long v);

  using snail_ptr = snail_number *;
  std::optional<long> value_;
  snail_ptr left_ = nullptr, right_ = nullptr, parent_ = nullptr;
};

snail_number::snail_number(std::istringstream &iss, snail_number *parent): parent_{parent} {
  iss.ignore();//"[";
  if (iss.peek() == '[') {
    auto lnode = new snail_number(iss, this);
    left_ = lnode;
  } else {
    long value;
    iss >> value;
    left_ = new snail_number(value, this);
  }
  iss.ignore();//",";
  if (iss.peek() == '[') {
    auto rnode = new snail_number(iss, this);
    right_ = rnode;
  } else {
    long value;
    iss >> value;
    right_ = new snail_number(value, this);
  }
  iss.ignore();//"]"
}
void snail_number::dump() const {
  if (value_) fmt::print("{}", *value_);
  else {
    fmt::print("[");
    left_->dump();
    fmt::print(",");
    right_->dump();
    fmt::print("]");
  }
}
snail_number *snail_number::find_explode_candidate(long depth) {
  if (depth == 5 && !value_)
    return this;

  if (!value_){
    auto left_path = left_->find_explode_candidate(depth + 1);
    return (left_path) ? left_path : right_->find_explode_candidate(depth + 1);
  }

  return nullptr;
}
void snail_number::perform_explosion(snail_number *target_node) {
  auto lv = target_node->left_->value_;
  auto rv = target_node->right_->value_;

  target_node->value_ = 0;
  delete(target_node->left_); target_node->left_ = nullptr;
  delete(target_node->right_); target_node->right_ = nullptr;

  auto parent = target_node->parent_;
  while (parent != nullptr) {
    if (parent->left_ == target_node) {
      if (rv) { parent->right_->add_left(*rv);  rv = {}; }
    }
    if (parent->right_ == target_node) {
      if (lv) { parent->left_->add_right(*lv); lv = {};}
    }
    target_node = parent;
    parent = parent->parent_;
  }
}

void snail_number::add_left(long v) {
  if (value_) *value_ += v;
  else
    left_->add_left(v);
}
void snail_number::add_right(long v) {
  if (value_) *value_ += v;
  else
    left_->add_right(v);
}

snail_number *snail_number::find_split_candidate() {
  if (value_)
    return (*value_ >= 10) ? this : nullptr;

  auto lr = left_->find_split_candidate();
  return (lr) ? lr : right_->find_split_candidate();
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  using namespace std::views;
  auto nodes = v | transform([](auto const& e){
                 std::istringstream iss{e.line};
                 return new snail_number(iss, nullptr);
               }) | transform([](auto e) { e->reduce(); return e; });

  auto sum = nodes[0];

  for (auto n : nodes | drop(1)){
    sum = new snail_number(sum, n);
    sum->reduce();
  }

  sum->dump();

  return 0;
}

AOC_Output part_2(std::vector<AOC_Input> const &v) { return 0; }

#include "../exec.hpp"