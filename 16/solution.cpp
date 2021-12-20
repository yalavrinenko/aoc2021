//
// Created by yalavrinenko on 02.12.2020.
//
#include "../main.hpp"
#include <variant>

struct AOC_Output {
  long value;
  AOC_Output(long value) : value(value) {}
  auto operator()() const { return value; }
};

struct AOC_Input {
  std::string line;
  explicit AOC_Input(std::string s) : line{std::move(s)} {}
};

class Utils {
public:
  static auto const &encoding_map() {
    static std::unordered_map<char, std::string> map{
        {'0', "0000"}, {'1', "0001"}, {'2', "0010"}, {'3', "0011"}, {'4', "0100"}, {'5', "0101"}, {'6', "0110"}, {'7', "0111"},
        {'8', "1000"}, {'9', "1001"}, {'A', "1010"}, {'B', "1011"}, {'C', "1100"}, {'D', "1101"}, {'E', "1110"}, {'F', "1111"},
    };
    return map;
  }
  static auto create_bitstream(std::string const &line) {
    std::stringstream ss;
    for (auto c: line) ss << encoding_map().at(c);
    return ss;
  }

  static long read_value(std::stringstream &iss, unsigned long bytes) {
    auto value = 0l;
    while (bytes > 0) {
      char c;
      iss >> c;
      value = (value << 1) + (c - '0');
      --bytes;
    }
    return value;
  }
};

class BITS_packet {
public:
  struct header {
    long version;
    long type;
  };

  struct operator_paket {
    std::vector<BITS_packet> subpackets;
  };

  explicit BITS_packet(std::stringstream &iss);

  [[nodiscard]] auto const &head() const { return head_; }
  [[nodiscard]] auto const &data() const { return data_; }

private:
  header head_{};
  std::variant<long, operator_paket> data_;
};

BITS_packet::BITS_packet(std::stringstream &iss) {
  head_.version = Utils::read_value(iss, 3);
  head_.type = Utils::read_value(iss, 3);
  if (!iss) return;
  if (head_.type == 4) {
    char lead_bit;
    std::stringstream value_ss;
    auto count = 0ul;
    do {
      iss >> lead_bit;
      for (auto i = 0; i < 4; ++i) {
        char c;
        iss >> c;
        value_ss << c;
      }
      count += 4;
    } while (lead_bit == '1');
    data_ = Utils::read_value(value_ss, count);
  } else {
    char length_id;
    iss >> length_id;

    auto length = (length_id == '0') ? Utils::read_value(iss, 15) : Utils::read_value(iss, 11);
    operator_paket op;
    if (length_id == '0') {
      std::unique_ptr<char[]> subpacket(new char[length]);
      iss.read(subpacket.get(), length);
      std::stringstream packet_stream;
      packet_stream.write(subpacket.get(), length);

      while (packet_stream) {
        auto sp = BITS_packet(packet_stream);
        if (packet_stream) op.subpackets.emplace_back(sp);
      }
    } else if (length_id == '1') {
      while (length > 0) {
        op.subpackets.emplace_back(iss);
        --length;
      }
    }
    data_ = op;
  }
}

std::optional<AOC_Input> AOC_Reader::create_from_string(const std::string &line) { return AOC_Input(line); }

unsigned long sum_packet_versions(BITS_packet const &p) {
  auto visitor = [](auto &&field) {
    using T = std::decay_t<decltype(field)>;
    if constexpr (std::is_same_v<T, long>) {
      return 0ul;
    } else if constexpr (std::is_same_v<T, BITS_packet::operator_paket>) {
      auto sum = 0ul;
      for (auto const &sp: field.subpackets) { sum += sum_packet_versions(sp); }
      return sum;
    }
  };
  return p.head().version + std::visit(visitor, p.data());
}

AOC_Output part_1(std::vector<AOC_Input> const &v) {
  auto ss = Utils::create_bitstream(v.front().line);
  BITS_packet packet(ss);

  return sum_packet_versions(packet);
}

class Executor {
public:
  static unsigned long evaluate_expression(BITS_packet const &packet);

private:
  static auto &operation_map();
};

auto &Executor::operation_map() {
  using function_t = std::function<unsigned long(BITS_packet const &)>;
  static std::unordered_map<long, function_t> map;

  auto subpacket = [](auto const &p) -> auto const & { return std::get<BITS_packet::operator_paket>(p.data()).subpackets; };

  map[4] = [](BITS_packet const &p) { return std::get<long>(p.data()); };

  map[0] = [subpacket](BITS_packet const &pack) {
    auto sum = 0ul;
    for (auto const &p: subpacket(pack)) sum += Executor::evaluate_expression(p);
    return sum;
  };

  map[1] = [subpacket](BITS_packet const &pack) {
    auto sum = 1ul;
    for (auto const &p: subpacket(pack)) sum *= Executor::evaluate_expression(p);
    return sum;
  };

  map[2] = [subpacket](BITS_packet const &pack) {
    auto min = std::ranges::min(subpacket(pack) | std::views::transform(&Executor::evaluate_expression));
    return min;
  };

  map[3] = [subpacket](BITS_packet const &pack) {
    auto max = std::ranges::max(subpacket(pack) | std::views::transform(&Executor::evaluate_expression));
    return max;
  };

  map[5] = [subpacket](BITS_packet const &pack) {
    return Executor::evaluate_expression(subpacket(pack)[0]) > Executor::evaluate_expression(subpacket(pack)[1]);
  };

  map[6] = [subpacket](BITS_packet const &pack) {
    return Executor::evaluate_expression(subpacket(pack)[0]) < Executor::evaluate_expression(subpacket(pack)[1]);
  };

  map[7] = [subpacket](BITS_packet const &pack) {
    return Executor::evaluate_expression(subpacket(pack)[0]) == Executor::evaluate_expression(subpacket(pack)[1]);
  };

  return map;
}
unsigned long Executor::evaluate_expression(const BITS_packet &packet) {
  return Executor::operation_map()[packet.head().type](packet);
}

AOC_Output part_2(std::vector<AOC_Input> const &v) {
  auto ss = Utils::create_bitstream(v.front().line);
  BITS_packet packet(ss);

  return Executor::evaluate_expression(packet);
}

#include "../exec.hpp"