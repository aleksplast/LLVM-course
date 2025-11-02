#include "CPU.hpp"
#include <iostream>

namespace ASM2IR {
    CPU *CPU::cpu;

void CPU::dump_cpu_state() const {
    std::cout << "pc: " << std::hex << pc << '\n';
    std::cout << "next_pc: " << next_pc << '\n';
    for (uint32_t reg_i = 0; reg_i < kRegNum; ++reg_i) {
        std::cout << "x" << reg_i << ": " << reg_file[reg_i] << '\n';
    }
    std::cout << '\n';
}
} // namespace ASM2IR
