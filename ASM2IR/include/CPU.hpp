#pragma once

#include <cinttypes>
#include <cstring>
#include <iostream>

extern "C" {
  int sim_rand();
  void screen_put_pixel(int x, int y, int argb);
  void screen_flush();
}

namespace ASM2IR {

struct CPU final {
  static constexpr uint32_t kRegNum = 64;
  long long reg_file[kRegNum] = {};
  uint32_t pc;
  uint32_t next_pc;
  bool run = true;

  static CPU *cpu;
  static void set_cpu(CPU *c) { cpu = c; }
  void dump_cpu_state() const;
#define ISA(Opcode_, Name_, SkipArgs_, ReadArgs_, WriteArgs_, Execute_,        \
             IRGenExecute_)                                                    \
static void do_##Name_(long long rd, long long rs1, long long rs2imm, long long rs3imm) {           \
    Execute_;                                                                  \
}
#include "ISA.hpp"
#undef ISA
};

} // namespace ASM2IR
