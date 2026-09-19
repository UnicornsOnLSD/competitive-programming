#include <charconv>
#include <cstdint>
#include <format>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

class IntcodeCpu {
public:
  uint64_t pc = 0;
  bool has_halted = false;
  std::vector<uint64_t> memory;

  IntcodeCpu(const std::string &program) {
    for (const auto opcode_str : std::views::split(program, ',')) {
      uint64_t opcode;

      auto [ptr, ec] = std::from_chars(
          opcode_str.data(), opcode_str.data() + opcode_str.size(), opcode);

      if (ec != std::errc{}) {
        throw std::system_error(std::make_error_code(ec));
      }

      memory.push_back(opcode);
    }
  }

  void dump_memory() {
    for (const auto &opcode : memory) {
      std::cout << opcode << ',';
    }

    std::cout << '\n';
  }

  void run_until_halt() {
    while (!has_halted) {
      step();
    }
  }

  void step() {
    switch (memory[pc]) {
    case 1:
      add();
      break;
    case 2:
      mult();
      break;
    case 99:
      halt();
      break;
    default:
      throw std::logic_error(std::format("Invalid instruction {}", memory[pc]));
    }
  }

private:
  void add() {
    auto a = memory[memory[pc + 1]];
    auto b = memory[memory[pc + 2]];

    memory[memory[pc + 3]] = a + b;
    pc += 4;
  }

  void mult() {
    auto a = memory[memory[pc + 1]];
    auto b = memory[memory[pc + 2]];

    memory[memory[pc + 3]] = a * b;
    pc += 4;
  }

  void halt() { has_halted = true; }
};

int main(int argc, char *argv[]) {
  std::string opcodes_str;
  std::getline(std::cin, opcodes_str);

  auto cpu = IntcodeCpu(opcodes_str);

  cpu.memory[1] = 12;
  cpu.memory[2] = 2;
  
  cpu.run_until_halt();

  std::cout << cpu.memory[0] << '\n';
}
