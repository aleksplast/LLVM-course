#pragma once

#include <cinttypes>
#include <cstring>
#include <iostream>
#include <memory>
#include <deque>
#include "Parser.hpp"

extern "C" {
    int  sim_rand();
    void screen_put_pixel(int x, int y, int argb);
    void screen_flush();
    void dump_grid(int *grid_ptr);
    void dump_neighbours(int *neigh_ptr, int x, int y);
    void dump_reg(long long reg_val);
}

namespace KIRCulator {

struct CPU final {
    static constexpr uint32_t kRegNum = 16;
    static const size_t kStackSize = 16UL * 1024 * 1024;

    long long reg_file[kRegNum] = {};
    std::deque<std::string> callstack;

    uint32_t pc = 0;
    uint32_t next_pc = 0;
    bool run = true;

    std::unique_ptr<uint8_t[]> stack = std::make_unique<uint8_t[]>(kStackSize);

    static CPU *cpu;
    static void set_cpu(CPU *c) { cpu = c; }

    void dump_cpu_state() const;

    void exec(const Parser &parser);

#define ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_,        \
            IRGenExecute_)                                                     \
    static void do_##Name_(long long r1, long long r2, long long r3imm) {     \
        cpu->next_pc += 1;                                                      \
        Execute_;                                                              \
    }
#include "ISA.hpp"
#undef ISA
};

} // namespace KIRCulator
