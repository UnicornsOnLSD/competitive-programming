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
    IntcodeCpu(const std::string &program_str)
        : IntcodeCpu(parse_program_string(program_str)) {}

    IntcodeCpu(std::vector<uint64_t> program) : memory(std::move(program)) {}

    // Getters and setters 😍

    /// Returns the "output" of the program, which AoC seems to use as position
    /// 0.
    uint64_t get_output() { return memory[0]; }

    uint64_t get_noun() { return memory[1]; }
    void set_noun(uint64_t val) { memory[1] = val; }

    uint64_t get_verb() { return memory[2]; }
    void set_verb(uint64_t val) { memory[2] = val; }

    // Debug functions

    /// Prints out the program's memory to standard output.
    void dump_memory() {
        bool is_first = true;

        for (const auto &opcode : memory) {
            if (!is_first) {
                std::cout << ',';
            }

            is_first = false;

            std::cout << opcode;
        }

        std::cout << '\n';
    }

    // Control functions

    /// Runs the program until it reaches a halt instruction.
    void run_until_halt() {
        while (!has_halted) {
            step();
        }
    }

    /// Parses a program string (from AoC input) into a vector of opcodes.
    static std::vector<uint64_t>
    parse_program_string(const std::string &program_str) {
        std::vector<uint64_t> out;

        for (const auto opcode_str : std::views::split(program_str, ',')) {
            uint64_t opcode;

            auto [ptr, ec] =
                std::from_chars(opcode_str.data(),
                                opcode_str.data() + opcode_str.size(), opcode);

            if (ec != std::errc{}) {
                throw std::system_error(std::make_error_code(ec));
            }

            out.push_back(opcode);
        }

        return out;
    }

  private:
    /// Instruction pointer
    uint64_t ip = 0;

    /// Whether or not we've reached a halt instruction
    bool has_halted = false;

    /// The program's memory
    std::vector<uint64_t> memory;

    // Control functions

    /// Run the next instruction in the program
    void step() {
        switch (memory[ip]) {
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
            throw std::logic_error(
                std::format("Invalid instruction {}", memory[ip]));
        }
    }

    // Instructions (in order of opcode number)

    /// Adds the value of the addresses in operand A and B into the address
    /// specified by operand C.
    void add() {
        auto a = memory[memory[ip + 1]];
        auto b = memory[memory[ip + 2]];

        memory[memory[ip + 3]] = a + b;
        ip += 4;
    }

    /// Multiplies the value of the addresses in operand A and B into the
    /// address specified by operand C.
    void mult() {
        auto a = memory[memory[ip + 1]];
        auto b = memory[memory[ip + 2]];

        memory[memory[ip + 3]] = a * b;
        ip += 4;
    }

    /// Halts the CPU
    void halt() { has_halted = true; }
};

int main(int argc, char *argv[]) {
    std::string program_str;
    std::getline(std::cin, program_str);
    auto program = IntcodeCpu::parse_program_string(program_str);

    auto cpu = IntcodeCpu(program);

    cpu.set_noun(12);
    cpu.set_verb(2);

    cpu.run_until_halt();

    std::cout << cpu.get_output() << '\n';

    for (int noun = 0; noun < 100; noun++) {
        for (int verb = 0; verb < 100; verb++) {
            cpu = IntcodeCpu(program);

            cpu.set_noun(noun);
            cpu.set_verb(verb);

            cpu.run_until_halt();

            if (cpu.get_output() == 19690720) {
                std::cout << 100 * noun + verb << '\n';
                return 0;
            }
        }
    }

    std::cout << "Failed to find part 2 answer :(\n";
    return 1;
}
